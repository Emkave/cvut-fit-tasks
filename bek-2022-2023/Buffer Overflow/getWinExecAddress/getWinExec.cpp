#include <iostream>
#include <string>
#include <Windows.h>

int main() {
    std::string stopper = "";
    while (1) {
        std::cout << "reload or exit, write one\n";
        std::cin >> stopper;

        if (stopper == "exit") {
            break;
        } else if (stopper == "reload") {
            HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
            FARPROC pWinExec = GetProcAddress(hKernel32, "WinExec");

            std::cout << "Address of WinExec: " << pWinExec << std::endl << std::endl;
        }

    }

    return 0;
}
