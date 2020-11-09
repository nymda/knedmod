// dllmain.cpp : Defines the entry point for the DLL application.
#include "mem.h"
#include <sstream>
#include <iostream>
#include <iomanip>

#define CHAR_NOT_SELECTED "="
#define CHAR_SELECTED ">"

void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

DWORD WINAPI main(HMODULE hModule)
{
    //make console window and enable stdout
    int selectedIndex = 0;
    AllocConsole();
    std::wstring strW = L"Internal diddler";
    SetConsoleTitle(strW.c_str());
    DeleteMenu(GetSystemMenu(GetConsoleWindow(), false), SC_CLOSE, MF_BYCOMMAND);
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hidecursor();
    std::cout.precision(2);

    HANDLE mainHandle = GetModuleHandle(L"teardown.exe");
    uintptr_t moduleBase = (uintptr_t)mainHandle;
    uintptr_t game = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x0 });
    uintptr_t player = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0xA0, 0x0 });
    uintptr_t scene = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x40, 0x0 });

    std::cout << "Game: " << std::hex << game << std::endl;
    std::cout << "Player: " << std::hex << player << std::endl;
    std::cout << "Scene: " << std::hex << scene << std::endl;

    //handles only activating a function on keyDown [uparrow, downarrow, rightarrow, q, i, k]
    bool kpHandler[6] = { true, true, true, true, true, true };

    //handles if a cheat is enabled / disabled
    bool cheatHandler[5] = { false, false, false, false, false };

    //contains cursor position
    int selectedItem = 0;

    //contains teleport location
    byte teleportPosition[12];
    float* x = (float*)(player);
    float* y = (float*)(player+4);
    float* z = (float*)(player+8);

    float savedX = 0.00;
    float savedY = 0.00;
    float savedZ = 0.00;

    while (true) {

        //draw the menu

        COORD tl = { 0,0 };
        SetConsoleCursorPosition(hConsole, tl);
        SetConsoleTextAttribute(hConsole, 15);

        std::cout << "Teleport position: ";
        SetConsoleTextAttribute(hConsole, 14);
        std::cout << savedX << " : " << savedY << " : " << savedZ << " (Save [I] Load [K])    " << std::endl;

        SetConsoleTextAttribute(hConsole, 15);

        if (selectedItem == 0) { std::cout << CHAR_SELECTED; }
        else{ std::cout << CHAR_NOT_SELECTED; }
        std::cout << "Godmode: ";
        if (cheatHandler[0]) {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "Enabled    " << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "Disabled    " << std::endl;
        }

        SetConsoleTextAttribute(hConsole, 15);

        if (selectedItem == 1) { std::cout << CHAR_SELECTED; }
        else { std::cout << CHAR_NOT_SELECTED; }
        std::cout << "Slow motion: ";
        if (cheatHandler[1]) {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "Enabled [Q]   " << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "Disabled [Q]   " << std::endl;
        }

        SetConsoleTextAttribute(hConsole, 15);

        if (selectedItem == 2) { std::cout << CHAR_SELECTED; }
        else { std::cout << CHAR_NOT_SELECTED; }
        std::cout << "Jetpack: ";
        if (cheatHandler[2]) {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "Enabled    " << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "Disabled    " << std::endl;
        }

        SetConsoleTextAttribute(hConsole, 15);

        if (selectedItem == 3) { std::cout << CHAR_SELECTED; }
        else { std::cout << CHAR_NOT_SELECTED; }
        std::cout << "Speedhack: ";
        if (cheatHandler[3]) {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "Enabled    " << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "Disabled    " << std::endl;
        }

        //handle keypresses

        if (((GetAsyncKeyState(VK_UP) & 0x0001) != 0)) {
            if (kpHandler[0]) {
                if (selectedItem > 0) {
                    selectedItem--;
                }
                kpHandler[0] = false;
            }
        }
        else {
            kpHandler[0] = true;
        }

        if (((GetAsyncKeyState(VK_DOWN) & 0x0001) != 0)) {
            if (kpHandler[1]) {
                if (selectedItem < 3) {
                    selectedItem++;
                }
                kpHandler[1] = false;
            }
        }
        else {
            kpHandler[1] = true;
        }

        if (((GetAsyncKeyState(VK_RIGHT) & 0x0001) != 0)) {
            if (kpHandler[2]) {
                cheatHandler[selectedItem] = !cheatHandler[selectedItem];
                kpHandler[2] = false;
            }
        }
        else {
            kpHandler[2] = true;
        }

        //Q
        if (((GetAsyncKeyState(0x51) & 0x0001) != 0)) {
            if (kpHandler[3]) {
                cheatHandler[1] = !cheatHandler[1];
                kpHandler[3] = false;
            }
        }
        else {
            kpHandler[3] = true;
        }

        //I
        if (((GetAsyncKeyState(0x49) & 0x0001) != 0)) {
            if (kpHandler[4]) {
                savedX = *x;
                savedY = *y;
                savedZ = *z;
                kpHandler[4] = false;
            }
        }
        else {
            kpHandler[4] = true;
        }

        //K
        if (((GetAsyncKeyState(0x4B) & 0x0001) != 0)) {
            if (kpHandler[5]) {
                *x = savedX;
                *y = savedY;
                *z = savedZ;
                kpHandler[5] = false;
            }
        }
        else {
            kpHandler[5] = true;
        }

        Sleep(16.6);
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