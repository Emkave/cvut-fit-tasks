import time
import datetime
import subprocess
import socket
import threading
import os
import configparser
import errno


def checkpoint():
    print("Checkpoint.");


def ReadConfig():
    config = configparser.ConfigParser();
    try:
        setpath = os.path.join(os.path.dirname(os.path.abspath(__file__)), "config.ini");
        config.read(setpath);
        port = config.getint('Server', 'port')
        if port < 0 or port > 65535:
            raise ValueError("Port must be between 0 and 65535")

    except Exception as e:
        print(f"Error while reading config.ini: {e}");

    return config;


class Server:
    def __init__(self, server_port):          # Initialize the server
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
        self.server_socket.setblocking(False);
        self.server_ip = socket.gethostbyname(socket.gethostname());
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1);
        self.to_shutdown = False;
        self.capacity = 3;
        self.clients = {};
        self.server_port = server_port;       # search for opened port
        self.server_socket.bind((self.server_ip, self.server_port));
        self.server_socket.listen(5);

        time.sleep(1);
        print("Initialized.");

        self.accept_thread = threading.Thread(target=self.AcceptClients);   # give a thread to accept connections
        print(f"Server's config: IP: {self.server_ip}, Port: {self.server_port}")       #


    def GetTime(self, csocket: socket.socket) -> str:
        if self.clients[csocket] == "standard":
            date = datetime.datetime.now();
            return f"Time:\nYear: {date.year}\nMonth: {date.month}\nDay: {date.day}\nHour: {date.hour}\nMinute: {date.minute}\nSecond: {date.second}\n";

        return datetime.datetime.now().strftime(self.clients[csocket]);


    def SetTimeFormat(self, csocket: socket.socket, format_str: str):       #A mini-compiler designed to make you display a time in various formats as you want.
        print("Setting time format...")         # I have completely forgot how it works now, but it is fun.
        keywords = {        #Disctionary of time keywords
            "day"   :      "%d",
            "month" :    "%m",
            "year"  :     "%Y",
            "hour"  :     "%H",
            "minute":   "%M",
            "second":   "%S",
        }

        result = ""
        i = 0

        try:
            while i < len(format_str):          #Scanning for the entire format you provided.
                if format_str[i] == "\\":       # here we are mostly checking the \ character, because it is cursed character.
                    if i + 1 < len(format_str):
                        if format_str[i + 1] == "n":    #Check for \n
                            result += "\n"
                        else:
                            result += format_str[i + 1] # Else move next
                        i += 1
                        if format_str[i] == "\\":       # Check again just in case
                            i += 1
                elif format_str[i] == "/":          # Check if we are about to enter a keyword
                    keyword = ""
                    i += 1
                    while i < len(format_str) and format_str[i] != "/":         #detecting keywords
                        keyword += format_str[i]
                        i += 1

                    if i >= len(format_str):
                        raise ValueError("Missing closing slash")

                    if keyword in keywords:
                        result += keywords[keyword]
                    else:
                        raise ValueError(f"Incorrect keyword or extra characters: {keyword}")
                else:
                    result += format_str[i]
                i += 1
        except Exception as e:
            print(f"Invalid time format provided: {e}");
            return;

        self.clients[csocket] = result;



    def AcceptClients(self):                # This is used by a separate thread to listen for connections
        while not self.to_shutdown:
            try:
                client_socket, client_address = self.server_socket.accept()
                client_socket.setblocking(True)  # Set the client socket to blocking
                client_ip, client_port = client_address

                if len(self.clients) > self.capacity:
                    try:
                        capacity_message = "Server is at full capacity. Please try again later."
                        client_socket.sendall(capacity_message.encode())
                        client_socket.close()
                        print(f"Server is full. Rejected connection from {client_ip}:{client_port}.")
                    except socket.error as e:
                        print(f"Error sending capacity message to client {client_ip}:{client_port}: {e}")
                else:
                    print(f"Client {client_ip}:{client_port} connected.")
                    self.clients[client_socket] = "standard";
                    handle_client_thread = threading.Thread(target=self.HandleClient, args=(client_socket,)) #We give each client a thread to solve their requestt
                    handle_client_thread.start()
                    client_socket.sendall("connected".encode());
                    self.SendTime(client_socket);
            except socket.error as e:
                if e.errno != errno.EAGAIN and e.errno != errno.EWOULDBLOCK:
                    print(f"Error accepting clients: {e}")
                time.sleep(0.1)  # Add a small sleep to avoid high CPU usage



    def HandleClient(self, client_socket):                      #Client handler. It works by a separate thread. it accepts packets from clients
        try:
            while True:
                data = client_socket.recv(1024)
                if data:
                    self.ParseCommand(data.decode(), client_socket)
                else:
                    break
        except Exception as e:
            pass


    def SendTime(self, client_socket: socket.socket):           #just send date info to the client
        print("Sending time data...");
        try:
            message = f"time {self.GetTime(client_socket)}";
            client_socket.sendall(message.encode());
        except Exception as e:
            print(f"Couldn't send server time");
        print("Time data sent successfully.");


    def ParseCommand(self, message: str, client_socket):        #almost the same parser as for the client, but here we also extract the client's socket to send him back messages
        tokens = message.split(' ')

        if tokens[0] == "gettime":
            self.SendTime(client_socket);
        elif tokens[0] == "setformat":
            self.SetTimeFormat(client_socket, tokens[1]);


class Client:
    def __init__(self):
        self.to_exit = False;
        self.time_format: str = "standard";
        print("Setting up client's socket...");
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
        print("Setting up ip address...");
        self.client_ip = socket.gethostbyname(socket.gethostname());
        print("Setting up port address...\n");
        self.client_port = 12345;
        self.is_connected = False;
        #print(f"Time: {self.GetTime()}");


    def FormatTime(self) -> str:                            #Here we print the date in some format. The standard one is provided as well.
        if self.time_format == "standard":
            now = datetime.datetime.now();
            return f"Year: {now.year}\nMonth: {now.month}\nDay: {now.day}\nHour: {now.hour}\nMinute: {now.minute}\nSecond: {now.second}";
        else:
            return datetime.datetime.now().strftime(self.time_format);


    def is_valid_date(self, year_, month, day):                 #Check for correct date
        try:
            datetime.date(year_, month, day)
            return True
        except ValueError:
            return False


    def ParseTimeString(self, time_str: str) -> tuple:
        time_components = time_str.split("|")
        if len(time_components) != 6:
            print("Invalid time string format")
            return ()

        for i in time_components:
            if str(i) == ' ':
                print("Couldn't set the time. Syntax error.")
                return ()

        default_time = [1971, 1, 1, 0, 0, 0]

        for i, component in enumerate(time_components):
            if component != "":
                try:
                    value = int(component)
                    if i == 0 and (value < 1971 or value > 2100):   # Check if a year is violating borders
                        raise ValueError(f"Invalid year: {component}")
                    elif i == 1 and (value < 1 or value > 12):         #Check same for month
                        raise ValueError(f"Invalid month: {component}")
                    default_time[i] = value  # Update default_time with parsed year and month

                    if i == 2 and not self.is_valid_date(default_time[0], default_time[1], value):  #check same for day
                        raise ValueError(f"Invalid day: {component}")
                    elif i == 3 and (value < 0 or value > 23):
                        raise ValueError(f"Invalid hour: {component}")#check for hours
                    elif (i == 4 or i == 5) and (value < 0 or value > 59):  #check for seconds and minutes
                        raise ValueError(f"Invalid {'minute' if i == 4 else 'second'}: {component}")
                except ValueError:
                    raise ValueError(f"Invalid time component: {component}")

        return tuple(default_time)


    def Help(self):
        print("User priveleged commands below: ")
        print("> exit                       -    Exit the program");
        print("> help                       -    Open this helping thing");
        print("> get time                   -    Get system time");
        print("> set time format <format>   -    Set the time format to be printed.")
        print("> connect <ip> <port>        -    Connect to server");
        print("> disconnect                 -    Disconnect from server");
        print("> shutdown                   -    Shutdown server");
        print("> start                      -    Start server");
        print("\nAdmin priveleged commands below: ")
        print("> set time <year|month|day|hour|minute|second>           -    Edit OS system time");


    def SetTimeFormat(self, format_str: str):       #A mini-compiler designed to make you display a time in various formats as you want.
        print("Setting time format...")         # I have completely forgot how it works now, but it is fun.
        keywords = {        #Disctionary of time keywords
            "day"   :      "%d",
            "month" :    "%m",
            "year"  :     "%Y",
            "hour"  :     "%H",
            "minute":   "%M",
            "second":   "%S",
        }

        result = ""
        i = 0

        try:
            while i < len(format_str):          #Scanning for the entire format you provided.
                if format_str[i] == "\\":       # here we are mostly checking the \ character, because it is cursed character.
                    if i + 1 < len(format_str):
                        if format_str[i + 1] == "n":    #Check for \n
                            result += "\n"
                        else:
                            result += format_str[i + 1] # Else move next
                        i += 1
                        if format_str[i] == "\\":       # Check again just in case
                            i += 1
                elif format_str[i] == "/":          # Check if we are about to enter a keyword
                    keyword = ""
                    i += 1
                    while i < len(format_str) and format_str[i] != "/":         #detecting keywords
                        keyword += format_str[i]
                        i += 1

                    if i >= len(format_str):
                        raise ValueError("Missing closing slash")

                    if keyword in keywords:
                        result += keywords[keyword]
                    else:
                        raise ValueError(f"Incorrect keyword or extra characters: {keyword}")
                else:
                    result += format_str[i]
                i += 1
        except Exception as e:
            print(f"Invalid time format provided: {e}");
            return;

        self.time_format = result;
        print("Time format setted successfully.");


    def SetTime(self, time: str):         #Noramal self-desrcibing command.
        t = self.ParseTimeString(time);

        if t == ():
            print("Couldn't set the time. Incorrect format.")
            return

        time_tuple_str = ",".join(map(str, t))
        setpath = os.path.join(os.path.dirname(os.path.abspath(__file__)), "set.py");
        cmd = ["sudo", "-k", "python3", setpath, time_tuple_str]
        subprocess.run(cmd, check=True)

        if self.is_connected:
            self.GetTime()


    def GetTime(self):                 # JUST Printing time from server or OS
        if not self.is_connected:
            date = datetime.datetime.now();
            if self.time_format == "standard":
                print(
                    f"Time:\nYear: {date.year}\nMonth: {date.month}\nDay: {date.day}\nHour: {date.hour}\nMinute: {date.minute}\nSecond: {date.second}\n")
            else:
                print(date.strftime(self.time_format) + '\n');
            return;


        try:
            msg = f"gettime {self.client_socket.fileno()}";
            self.client_socket.sendall(msg.encode());
        except Exception as e:
            print(f"Error while requesting the time: {e}");


    def ConnectToServer(self, ip, port):
        try:
            socket.inet_aton(ip);
            if not (1 <= int(port) <= 65535):
                raise ValueError("Invalid port number.");

            self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
            self.client_socket.connect((ip, int(port)));

            msg = f"connect {self.client_socket.fileno()} standard";
            self.client_socket.sendall(msg.encode());
            acknwl = self.client_socket.recv(1024).decode();
            if acknwl == "connected":
                self.is_connected = True;
                print(f"Connected to the server at {ip}:{port}");
                self.recv_thread = threading.Thread(target=self.ReceiveData);
                self.recv_thread.start();
            else:
                print(acknwl);
                self.client_socket.close();
        except Exception as e:
            print(f"Error while connecting to the server: {e}");


    def ReceiveData(self):
        try:
            while self.is_connected:
                data = self.client_socket.recv(1024);
                if data:
                    msg = data.decode();
                    if msg[:4] == "time":
                        print(msg[4:]);
                    else:
                        print(f"Server: {msg}\n");
                else:
                    self.is_connected = False;
        except Exception as e:
            print(f"Error while receiving data from server: {e}");


    def ParseCommand(self, command: str):           # I guess this parser is self-explaining
        command_tokens = command.split()            # WE take commands and make tokens and just pass them throught the parser
        command_name = command_tokens[0]

        try:
            if command_name == "help":
                self.Help();
            elif command_name == "get":
                if len(command_tokens) == 2:
                    if command_tokens[1] == "time":
                        self.GetTime();
                    else:
                        print("Usage: get time");
                else:
                    print("Usage: get time");

            elif command_name == "set":
                if len(command_tokens) == 3 and command_tokens[1] == "time":
                    self.SetTime(command_tokens[2]);
                elif command_tokens[1] == "time" and command_tokens[2] == "format":
                    self.SetTimeFormat(" ".join(command_tokens[3:]));
                    if self.is_connected:
                        msg = f"setformat {self.time_format}";
                        self.client_socket.sendall(msg.encode());
                else:
                    print("Usage: set time <password> <year|month|day|hour|minute|second> or set time format <format>");
            elif command_name == "connect":
                if len(command_tokens) == 3:
                    self.ConnectToServer(command_tokens[1], command_tokens[2]);
                else:
                    print("Usage: connect <ip> <port>");
            else:
                print("The command does not exist.");
        except Exception as e:
            print(f"Unexpected error: {e}");


def main():
    print("Starting the client...")  # Add this line
    client: Client = Client();
    print("Starting the server...");

    try:
        server: Server = Server(int(ReadConfig()['Server']['port']));
        server.accept_thread.start();
    except Exception as e:
        print(f"Error while starting the server: {e}")

    print("Enter 'help' to get all commands.")

    while not client.to_exit:
        command: str = str(input(">> ")[:128].strip())      # Getting input from a user with some restricitions. The program will run untill it is not to exit

        if not command:
            print("Command not entered.")
            continue


        client.ParseCommand(command=command)        #Pass commands
        print("\n")

        time.sleep(2)

    print("Exited.")


if __name__ == "__main__":
    main();                 # Start here
