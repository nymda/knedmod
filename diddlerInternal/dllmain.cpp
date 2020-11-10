#define _WIN32_WINNT 0x0500
#include "mem.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#define CHAR_NOT_SELECTED "|"
#define CHAR_SELECTED ">"
#define BASE_GAME_SPEED 0.016f

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
    HWND console = GetConsoleWindow();
    DeleteMenu(GetSystemMenu(console, false), SC_CLOSE, MF_BYCOMMAND);
    SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hidecursor();
    std::cout.precision(2);
    SMALL_RECT tmp = { 0, 0, 60, 10 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &tmp);

    HANDLE mainHandle = GetModuleHandle(L"teardown.exe");
    uintptr_t moduleBase = (uintptr_t)mainHandle;
    uintptr_t game = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x0 });
    uintptr_t player = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0xA0, 0x0 });
    uintptr_t scene = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x40, 0x0 });

    //handles only ctivating a function on keyDown [uparrow, downarrow, rightarrow, q, i, k, space, leftarrow]
    bool kpHandler[7] = { true, true, true, true, true, true, true };
    bool kpHandler2[7] = { true, true, true, true, true, true, true };

    //handles if a cheat is enabled / disabled
    bool prevCheatHandler[6] = { false, false, false, false, false, false };
    bool cheatHandler[6] = { false, false, false, false, false, false };

    //contains cursor position
    int selectedItem = 0;

    //misc player values
    byte teleportPosition[12];

    float* x = (float*)(player);
    float* y = (float*)(player + 4);
    float* z = (float*)(player + 8);
    float* yVelo = (float*)(player + 0x38 + 4);
    float* pHealth = (float*)(player + 0x015C);
    float* pSpeed = (float*)(player + 0x0160);

    //game vars and stuff
    float desiredSpeed = 1.0f;
    int desiredGameSpeedMultiple = 1;
    float desitedGameSpeed = BASE_GAME_SPEED;
    float* gamespeed = (float*)(game + 0x144);

    float setVelo = 0;

    float savedX = 0.00;
    float savedY = 0.00;
    float savedZ = 0.00;

    byte campos[84] = {};
    byte camang[84] = {};

    int i = 0;

    //patch basic instructions
    mem::Nop((byte*)(moduleBase + 0xA6182), 8); //stop setting health value
    mem::Patch((byte*)(moduleBase + 0xA618a), (byte*)"\xF3\x0F\x10\x81\x60\x01\x00\x00", 8); //health value patch
    mem::Nop((byte*)(moduleBase + 0x242D8), 10); //stop setting gamespeed

    while (true) {

        //draw the menu
        hidecursor();
        COORD tl = { 0,0 };
        SetConsoleCursorPosition(hConsole, tl);
        SetConsoleTextAttribute(hConsole, 15);

        std::cout << "Teleport position: ";
        SetConsoleTextAttribute(hConsole, 14);
        std::cout << savedX << " : " << savedY << " : " << savedZ << " (Save [I] Load [K])    " << std::endl;

        SetConsoleTextAttribute(hConsole, 15);

        if (selectedItem == 0) { std::cout << CHAR_SELECTED; }
        else { std::cout << CHAR_NOT_SELECTED; }
        std::cout << "Godmode         : ";
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
        std::cout << "Slow motion [" << desiredGameSpeedMultiple <<"] : ";
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
        std::cout << "Jetpack         : ";
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
        std::cout << "Speedhack   [" << desiredSpeed << "] : ";
        if (cheatHandler[3]) {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "Enabled    " << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "Disabled    " << std::endl;
        }

        SetConsoleTextAttribute(hConsole, 15);

        if (selectedItem == 4) { std::cout << CHAR_SELECTED; }
        else { std::cout << CHAR_NOT_SELECTED; }
        std::cout << "Del. walls      : ";
        if (cheatHandler[4]) {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "Enabled    " << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "Disabled    " << std::endl;
        }

        SetConsoleTextAttribute(hConsole, 15);

        std::cout << "Game: " << std::hex << game << std::endl;
        std::cout << "Player: " << std::hex << player << std::endl;
        std::cout << "Scene: " << std::hex << scene << std::endl;

		//handle keypresses

		//up
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

		//down
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

		//right
		if (((GetAsyncKeyState(VK_RIGHT) & 0x0001) != 0)) {
			if (kpHandler[2]) {
				cheatHandler[selectedItem] = !cheatHandler[selectedItem];
				kpHandler[2] = false;
			}
		}
		else {
			kpHandler[2] = true;
		}

		//left
		if (((GetAsyncKeyState(VK_LEFT) & 0x0001) != 0)) {
			if (kpHandler2[1]) {
                if (selectedItem == 3) {
                    desiredSpeed += 1.0f;
                    if (desiredSpeed > 9) {
                        desiredSpeed = 1;
                    }
                }
                else if (selectedItem == 1) {
                    desiredGameSpeedMultiple += 1;
                    desitedGameSpeed = BASE_GAME_SPEED / desiredGameSpeedMultiple;
                    if (desiredGameSpeedMultiple > 9) {
                        desiredGameSpeedMultiple = 1;
                    }
                }
                kpHandler2[1] = false;
			}
		}
		else {
			kpHandler2[1] = true;
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
				memcpy(campos, (void*)(player + 0x0060), sizeof campos);
				memcpy(campos, (void*)(player + 0x00C4), sizeof campos);
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
				memcpy((void*)(player + 0x0060), campos, sizeof campos);
				memcpy((void*)(player + 0x00C4), campos, sizeof campos);
				kpHandler[5] = false;
			}
		}
		else {
			kpHandler[5] = true;
		}

		//space & jetpack
		if (((GetAsyncKeyState(VK_SPACE) >> 15) & 0x0001) == 0x0001) {
			if (cheatHandler[2]) {
				if (kpHandler[6]) {
					setVelo = *yVelo;
					kpHandler[6] = false;
				}
				if (setVelo > 12)
				{
					setVelo = 12;
				}
				else
				{
					setVelo += 0.5f;
				}
				*yVelo = setVelo;
			}
		}
		else {
			kpHandler[6] = true;
		}

		//general cheat stuff

		if (cheatHandler[0] != prevCheatHandler[0]) {
			if (cheatHandler[0]) {
				//enable godmode
				mem::Nop((byte*)(moduleBase + 0xA6182), 8); // cant remember
				mem::Nop((byte*)(moduleBase + 0xA8CF3), 8); // basic damage
				mem::Nop((byte*)(moduleBase + 0xA7FB7), 8); // fall damage
				mem::Nop((byte*)(moduleBase + 0xA9E95), 8); // fire damage
				*pHealth = 1.0f; //set health to full
			}
			else {
				//disable godmode
				mem::Patch((byte*)(moduleBase + 0xA6182), (byte*)"\xF3\x0F\x11\x81\x5C\x01\x00\x00", 8); // ???
				mem::Patch((byte*)(moduleBase + 0xA8CF3), (byte*)"\xF3\x0F\x11\x91\x5C\x01\x00\x00", 8); // basic damage
				mem::Patch((byte*)(moduleBase + 0xA7FB7), (byte*)"\xF3\x0F\x11\x86\x5C\x01\x00\x00", 8); // fall damage
				mem::Patch((byte*)(moduleBase + 0xA9E95), (byte*)"\xF3\x0F\x11\x87\x5C\x01\x00\x00", 8); // fire damage
			}
		}

		//if speedhack is enabled then set speed to 5, if not then set speed to the health value
		if (cheatHandler[3]) {
			*pSpeed = (desiredSpeed);
		}
		else {
			*pSpeed = *pHealth;
		}

		if (cheatHandler[4] && *(byte*)(scene + 0x530) != 0x00) {
			mem::Null((byte*)(scene + 0x530), 8);
		}

        if (cheatHandler[1]) {
            *gamespeed = desitedGameSpeed;
        }
        else {
            *gamespeed = BASE_GAME_SPEED;
        }

		memcpy(prevCheatHandler, cheatHandler, sizeof cheatHandler);

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


