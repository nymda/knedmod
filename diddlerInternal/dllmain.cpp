// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "mem.h"

DWORD WINAPI main(HMODULE hModule)
{
    //get base address
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"teardown.exe");

    //make console window and enable stdout
    AllocConsole();
    std::wstring strW = L"Internal diddler";
    SetConsoleTitle(strW.c_str());
    DeleteMenu(GetSystemMenu(GetConsoleWindow(), false), SC_CLOSE, MF_BYCOMMAND);
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Console, because knedit cant make a GUI\n";

    //do the hacks and stuff
    while (true) {
        Sleep(10);
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

