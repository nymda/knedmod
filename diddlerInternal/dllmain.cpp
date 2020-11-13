#define _WIN32_WINNT 0x0500
#include "mem.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <ctime>
#define CHAR_NOT_SELECTED " "
#define CHAR_SELECTED ">"
#define BASE_GAME_SPEED 0.016f

class Vector3 {
public:
    float x;
    float y;
    float z;
};

void hidecursor();
void explosionPatch(float explosionMultiplier, uintptr_t moduleBase);
void pewpewPatch(float pewMultiplier, uintptr_t moduleBase);

DWORD WINAPI main(HMODULE hModule)
{
    //number of cheats for menu
    int CHEAT_COUNT = 7;

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
    SMALL_RECT tmp = { 0, 0, 60, 15 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &tmp);

    HANDLE mainHandle = GetModuleHandle(L"teardown.exe");
    uintptr_t moduleBase = (uintptr_t)mainHandle;
    uintptr_t game = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x0 });
    uintptr_t player = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0xA0, 0x0 });
    uintptr_t scene = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x40, 0x0 });

    //handles only ctivating a function on keyDown [uparrow, downarrow, rightarrow, q, i, k, space, leftarrow]
    bool kpHandler[8] = { true, true, true, true, true, true, true, true };

    //handles if a cheat is enabled / disabled
    bool prevCheatHandler[12] = { false, false, false, false, false, false, false, false , false , false , false , false };
    bool cheatHandler[12] = { false, false, false, false, false, false, false, false , false , false , false , false };

    //contains cursor position
    int selectedItem = 0;

    //misc player values
    float* x = (float*)(player);
    float* y = (float*)(player + 4);
    float* z = (float*)(player + 8);
    float* yVelo = (float*)(player + 0x38 + 4);
    float* pHealth = (float*)(player + 0x015C);
    float* pSpeed = (float*)(player + 0x0160);
    Vector3 cpos = Vector3();

    //game vars and stuff
    float desiredSpeed = 1.0f;
    int desiredGameSpeedMultiple = 9;
    float desitedGameSpeed = BASE_GAME_SPEED;
    float* gamespeed = (float*)(game + 0x144);

    float setVelo = 0;

    float savedX = 0.00;
    float savedY = 0.00;
    float savedZ = 0.00;

    int explosionMultiplier = 1;
    int shootyMultiplier = 1;

    byte campos[84] = {};
    byte camang[84] = {};

    int i = 0;

    //patch basic instructions
    mem::Patch((byte*)(moduleBase + 0xA618a), (byte*)"\xF3\x0F\x10\x81\x60\x01\x00\x00", 8); //health value patch
    mem::Nop((byte*)(moduleBase + 0x242D8), 10); //stop setting gamespeed

    double elapsed_secs = 0;

    while (true) {

        clock_t begin = clock();
        cpos.x = *x;
        cpos.y = *x;
        cpos.z = *x;

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

        if (selectedItem == 5) { std::cout << CHAR_SELECTED; }
        else { std::cout << CHAR_NOT_SELECTED; }
        std::cout << "Big bombs   [" << explosionMultiplier << "] : ";
        if (cheatHandler[5]) {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "Enabled    " << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "Disabled    " << std::endl;
        }

        SetConsoleTextAttribute(hConsole, 15);

        if (selectedItem == 6) { std::cout << CHAR_SELECTED; }
        else { std::cout << CHAR_NOT_SELECTED; }
        std::cout << "Big guns    [" << shootyMultiplier << "] : ";
        if (cheatHandler[6]) {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "Enabled    " << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "Disabled    " << std::endl;
        }

        SetConsoleTextAttribute(hConsole, 15);

        if (selectedItem == 7) { std::cout << CHAR_SELECTED; }
        else { std::cout << CHAR_NOT_SELECTED; }
        std::cout << "No use cooldown : ";
        if (cheatHandler[7]) {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "Enabled    " << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "Disabled    " << std::endl;
        }

        SetConsoleTextAttribute(hConsole, 79);

        std::cout << std::endl;
        std::cout << "Base: 0x" << std::hex << moduleBase << std::endl;
        std::cout << "Game: 0x" << std::hex << game << std::endl;
        std::cout << "Player: 0x" << std::hex << player << std::endl;
        std::cout << "Scene: 0x" << std::hex << scene << std::endl;

		//handle keypresses

		//up
		if (((GetAsyncKeyState(VK_UP) >> 15) & 0x0001) == 0x0001) {
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
		if (((GetAsyncKeyState(VK_DOWN) >> 15) & 0x0001) == 0x0001) {
			if (kpHandler[1]) {
				if (selectedItem < CHEAT_COUNT) {
					selectedItem++;
				}
				kpHandler[1] = false;
			}
		}
		else {
			kpHandler[1] = true;
		}

		//right
		if (((GetAsyncKeyState(VK_RIGHT) >> 15) & 0x0001) == 0x0001) {
			if (kpHandler[2]) {
                //toggle selected cheat
				cheatHandler[selectedItem] = !cheatHandler[selectedItem];
				kpHandler[2] = false;
			}
		}
		else {
			kpHandler[2] = true;
		}

		//left
		if (((GetAsyncKeyState(VK_LEFT) >> 15) & 0x0001) == 0x0001) {
			if (kpHandler[7]) {
                //set player run speed
                if (selectedItem == 3) {
                    desiredSpeed += 1.0f;
                    if (desiredSpeed > 9) {
                        desiredSpeed = 1;
                    }
                }

                //set slow motion speed
                else if (selectedItem == 1) {
                    desiredGameSpeedMultiple += 1;             
                    if (desiredGameSpeedMultiple > 9) {
                        desiredGameSpeedMultiple = 1;
                    }
                }

                //set 'splode multiplier
                else if (selectedItem == 5) {
                    explosionMultiplier += 1;
                    if (explosionMultiplier > 9) {
                        explosionMultiplier = 1;
                    }
                    if (cheatHandler[5]) {
                        explosionPatch(explosionMultiplier, moduleBase);
                    }
                }

                //set 'hoot multiplier
                else if (selectedItem == 6) {
                    shootyMultiplier += 1;
                    if (shootyMultiplier > 9) {
                        shootyMultiplier = 1;
                    }
                    if (cheatHandler[6]) {
                        pewpewPatch(shootyMultiplier, moduleBase);
                    }
                }
                kpHandler[7] = false;
			}
		}
		else {
            kpHandler[7] = true;
		}

		//Q
		if (((GetAsyncKeyState(0x51) >> 15) & 0x0001) == 0x0001) {
			if (kpHandler[3]) {
				cheatHandler[1] = !cheatHandler[1];
				kpHandler[3] = false;
			}
		}
		else {
			kpHandler[3] = true;
		}

		//I
		if (((GetAsyncKeyState(0x49) >> 15) & 0x0001) == 0x0001) {
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
		if (((GetAsyncKeyState(0x4B) >> 15) & 0x0001) == 0x0001) {
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
                mem::Nop((byte*)(moduleBase + 0xA6182), 8); //stop setting health value
				mem::Nop((byte*)(moduleBase + 0xA8CF3), 8); // basic damage
				mem::Nop((byte*)(moduleBase + 0xA7FB7), 8); // fall damage
				mem::Nop((byte*)(moduleBase + 0xA9E95), 8); // fire damage
				*pHealth = 1.0f; //set health to full
			}
			else {
				//disable godmode
				mem::Patch((byte*)(moduleBase + 0xA6182), (byte*)"\xF3\x0F\x11\x81\x5C\x01\x00\x00", 8); // start setting health value
				mem::Patch((byte*)(moduleBase + 0xA8CF3), (byte*)"\xF3\x0F\x11\x91\x5C\x01\x00\x00", 8); // basic damage
				mem::Patch((byte*)(moduleBase + 0xA7FB7), (byte*)"\xF3\x0F\x11\x86\x5C\x01\x00\x00", 8); // fall damage
				mem::Patch((byte*)(moduleBase + 0xA9E95), (byte*)"\xF3\x0F\x11\x87\x5C\x01\x00\x00", 8); // fire damage
			}
		}

        //set game to either the custom speed or the base speed if slowmo is enabled or disabled
        if (cheatHandler[1]) {
            desitedGameSpeed = BASE_GAME_SPEED / desiredGameSpeedMultiple;
            *gamespeed = desitedGameSpeed;
        }
        else {
            *gamespeed = BASE_GAME_SPEED;
        }

		//if speedhack is enabled then set speed to 5, if not then set speed to the health value
		if (cheatHandler[3]) {
			*pSpeed = (desiredSpeed);
		}
		else {
			*pSpeed = *pHealth;
		}

        //if no walls is enabled and the first byte of the wall location is not null, delete the wallls
		if (cheatHandler[4] && *(byte*)(scene + 0x530) != 0x00) {
			mem::Null((byte*)(scene + 0x530), 8);
		}

        if (cheatHandler[5] != prevCheatHandler[5]) {
            if (cheatHandler[5]) {
                //enable bigger explosions

                //explosion API patches for other mods
                mem::Nop((byte*)(moduleBase + 0x112579), 8); //api min
                mem::Nop((byte*)(moduleBase + 0x112595), 8); //api max

                explosionPatch(explosionMultiplier, moduleBase);
            }
            else {
                //disable bigger explosions
                mem::Patch((byte*)(moduleBase + 0xC40DB), (byte*)"\xF3\x41\x0F\x59\xF1\xF3\x41\x0F\x5F\xF5\xF3\x41\x0F\x5D\xF3", 15); //unpatch 
                mem::Patch((byte*)(moduleBase + 0xA508C), (byte*)"\xF3\x0F\x59\xF7\xF3\x41\x0F\x5F\xF0\xF3\x41\x0F\x5D\xF4\xF3\x0F\x59\xF7\xF3\x0F\x58\x35\xA2\xAC\x24\x00", 18); //unpatch 
            }
        }

        if (cheatHandler[6] != prevCheatHandler[6]) {
            if (cheatHandler[6]) {
                //enable bigger guns
                pewpewPatch(explosionMultiplier, moduleBase);
            }
            else {
                //disable bigger guns
                mem::Patch((byte*)(moduleBase + 0xC3C88), (byte*)"\x0F\x28\xF8\xF3\x41\x0F\x5C\xFB\xF3\x41\x0F\x59\xF9\xF3\x41\x0F\x5F\xFD\xF3\x41\x0F\x5D\xFB", 23); //unpatch 
            }
        }

        if (cheatHandler[7] != prevCheatHandler[7]) {
            if (cheatHandler[7]) {
                //enable no cooldown
                mem::Nop((byte*)(moduleBase + 0xAE281), 8);
                mem::Nop((byte*)(moduleBase + 0xA44D9), 6);
            }
            else {
                //enable cooldown again
                mem::Patch((byte*)(moduleBase + 0xAE281), (byte*)"\xF3\x0F\x11\x81\x20\x04\x00\x00", 8);
                mem::Patch((byte*)(moduleBase + 0xA44D9), (byte*)"\x89\x87\x20\x04\x00\x00", 6);
            }
        }

		memcpy(prevCheatHandler, cheatHandler, sizeof cheatHandler);

        clock_t end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        //as the game runs at 60fps, run the cheat loop 60 times a second
		Sleep(16.6 - elapsed_secs);
    }
}

void explosionPatch(float explosionMultiplier, uintptr_t moduleBase) {
    float finStrength = (5 + (2 * explosionMultiplier));
    unsigned char const * p = reinterpret_cast<unsigned char const*>(&finStrength);
    byte bytecode_rocket[15] = { 0xBA, p[0], p[1], p[2], p[3], 0x66, 0x0F, 0x6E, 0xF2, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }; // (mov edx,[finStrength]) , (movd xmm6,edx)
    byte bytecode_bomb[26] = { 0xBA, p[0], p[1], p[2], p[3], 0x66, 0x0F, 0x6E, 0xF2, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }; // (mov edx,[finStrength]) , (movd xmm6,edx)
    mem::Patch((byte*)(moduleBase + 0xC40DB), (byte*)bytecode_rocket, 15);
    mem::Patch((byte*)(moduleBase + 0xA508C), (byte*)bytecode_bomb, 18);
}

void pewpewPatch(float pewMultiplier, uintptr_t moduleBase) {
    float finStrength = (5 + (2 * pewMultiplier));
    unsigned char const* p = reinterpret_cast<unsigned char const*>(&finStrength);
    byte bytecode_gun[23] = { 0xBA, p[0], p[1], p[2], p[3], 0x66, 0x0F, 0x6E, 0xFA, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }; // (mov edx,[finStrength]) , (movd xmm6,edx)
    mem::Patch((byte*)(moduleBase + 0xC3C88), (byte*)bytecode_gun, 23);
}

void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
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




