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
#include "mainHook.h"
#include "movementLoop.h"
#include "focusHook.h"
#include "noclip.h"

FILE* makeConsole() {
    AllocConsole();
    std::wstring strW = L"DEBUG";
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
    std::cout << " __  __ _______ _______ _____  _______ _______ _____  " << std::endl;
    std::cout << "|  |/  |    |  |    ___|     \\|   |   |       |     \\ " << std::endl;
    std::cout << "|     <|       |    ___|  --  |       |   -   |  --  |" << std::endl;
    std::cout << "|__|\\__|__|____|_______|_____/|__|_|__|_______|_____/ " << std::endl;
    std::cout << "For teardown MAIN BRANCH version 0.7.2" << std::endl;
    std::cout << "KnedMod is C to Knedit and is released under the GNU public license" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;

    glb::gWnd = FindWindow(0, L"Teardown");
    HANDLE mainHandleDebug = GetModuleHandle(L"teardownsteamless.exe");
    HANDLE mainHandleSteam = GetModuleHandle(L"teardown.exe");

    std::cout << "[I] GWND           : " << glb::gWnd << std::endl;
    std::cout << "[I] mainHandleDebug: " << mainHandleDebug << std::endl;
    std::cout << "[I] mainHandleSteam: " << mainHandleSteam << std::endl;

    if (mainHandleSteam != 0) {
        glb::moduleBase = (uintptr_t)mainHandleSteam;
    }
    else {
        glb::moduleBase = (uintptr_t)mainHandleDebug;
    }

    initSwapBuffersHook();

    std::cout << "[I] Hooked swapBuffers" << std::endl;

    initHIDsHook();

    std::cout << "[I] Hooked WNDPROC" << std::endl;

    sigscanItems();

    std::cout << "[I] Completed sigscanning" << std::endl;

    initTestHook();
    initGodmodeHook();
    focusHook::initFocusHook();

    //initMovementHook();

    while (true) {
        if (((GetAsyncKeyState(VK_END) >> 15) & 0x0001) == 0x0001) {
            if (true) {

                if (noclip::inNoclip) {
                    noclip::ToggleNoclip();
                }

                ////undo hooks
                fclose(cnsl);
                FreeConsole();

                terminateSwapBuffersHook();
                terminateMovementHook();
                terminateHIDsHook();
                terminateTestHook();
                terminateGodmodeHook();
                focusHook::terminateFocusHook();

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

