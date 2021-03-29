#define _WIN32_WINNT 0x0500
#define GLEW_STATIC
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "mem.h"
#include "glew.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <ctime>
#include "detours.h"
#include <mutex>
#include "global.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <random>
#include "swapBuffers.h"
#include "Global.h"
#include "TDFuncs.h"
#include "hookTests.h"
#include "Mods.h"

FILE* makeConsole() {
    AllocConsole();
    std::wstring strW = L"Gek";
    SetConsoleTitle(strW.c_str());
    HWND console = GetConsoleWindow();
    DeleteMenu(GetSystemMenu(console, false), SC_CLOSE, MF_BYCOMMAND);
    SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.precision(2);
    SMALL_RECT tmp = { 0, 0, 120, 15 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &tmp);
    return f;
}

DWORD WINAPI main(HMODULE hModule)
{
    glb::hMdl = hModule;
    FILE* cnsl = makeConsole();
    std::cout << "injected" << std::endl;

    glb::gWnd = FindWindow(0, L"Teardown");
    HANDLE mainHandle = GetModuleHandle(L"teardown.exe.unpacked.exe");
    glb::moduleBase = (uintptr_t)mainHandle;

    initSwapBuffersHook();
    initHIDsHook();
    sigscanItems();
    initTestHook();
    initGodmodeHook();

    while (true) {
        if (((GetAsyncKeyState(VK_END) >> 15) & 0x0001) == 0x0001) {
            if (true) {
                ////undo hooks
                fclose(cnsl);
                FreeConsole();

                terminateSwapBuffersHook();
                terminateHIDsHook();
                terminateTestHook();
                terminateGodmodeHook();

                //sleep
                Sleep(250);

                //exit
                FreeLibraryAndExitThread(hModule, 0);
            }
        }
        Sleep(1);
    }
}


//idk, dont touch this
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main, hModule, 0, nullptr));
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

