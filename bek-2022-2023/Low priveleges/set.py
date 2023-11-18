import ctypes
import sys
from ctypes.util import find_library
from datetime import datetime

def is_valid_date(year_, month, day):  # Check for correct date
    try:
        datetime(year_, month, day)
        return True
    except ValueError:
        return False

def parse_time_string(time_str: str) -> tuple:
    time_components = time_str.split(",")
    if len(time_components) != 6:
        raise ValueError("Invalid time string format")

    default_time = [1971, 1, 1, 0, 0, 0]

    for i, component in enumerate(time_components):
        try:
            value = int(component)
            if i == 0 and (value < 1971 or value > 2100):   # Check if a year is violating borders
                raise ValueError(f"Invalid year: {component}")
            elif i == 1 and (value < 1 or value > 12):         # Check same for month
                raise ValueError(f"Invalid month: {component}")
            default_time[i] = value  # Update default_time with parsed year and month

            if i == 2 and not is_valid_date(default_time[0], default_time[1], value):  # Check same for day
                raise ValueError(f"Invalid day: {component}")
            elif i == 3 and (value < 0 or value > 23):  # Check for hours
                raise ValueError(f"Invalid hour: {component}")
            elif (i == 4 or i == 5) and (value < 0 or value > 59):  # Check for seconds and minutes
                raise ValueError(f"Invalid {'minute' if i == 4 else 'second'}: {component}")
        except ValueError:
            raise ValueError(f"Invalid time component: {component}")

    return tuple(default_time)

def set_system_time(time_tuple):
    # Load the C library
    libc = ctypes.CDLL(find_library("c"))

    # Define the timeval struct
    class timeval(ctypes.Structure):
        _fields_ = [
            ("tv_sec", ctypes.c_long),
            ("tv_usec", ctypes.c_long),
        ]

    # Define the timezone struct
    class timezone(ctypes.Structure):
        _fields_ = [
            ("tz_minuteswest", ctypes.c_int),
            ("tz_dsttime", ctypes.c_int),
        ]

    # Set the new time
    new_time = int(datetime(*time_tuple).timestamp())

    # Populate the timeval struct
    t = timeval()
    t.tv_sec = new_time
    t.tv_usec = 0

    # Set the timezone
    tz = timezone()
    tz.tz_minuteswest = 0  # Adjust this value for your timezone
    tz.tz_dsttime = 0

    # Call the settimeofday function with root privileges
    result = libc.settimeofday(ctypes.pointer(t), ctypes.pointer(tz))

    if result != 0:
        print("Error: Failed to set the system time. Ensure you have root privileges.")
    else:
        print("System time successfully changed.")


if __name__ == "__main__":
    time_tuple_str = sys.argv[1][:32]
    try:
        time_tuple = parse_time_string(time_tuple_str)
        set_system_time(time_tuple)
    except ValueError as e:
        print(f"Error: {e}")
