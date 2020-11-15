#define _WIN32_WINNT 0x0500
#define GLEW_STATIC

#include "mem.h"
#include "glew.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <ctime>
#include "detours.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#include <mutex>

#define CHAR_NOT_SELECTED " "
#define CHAR_SELECTED ">"
#define BASE_GAME_SPEED 0.016f

#pragma comment(lib, "glew32s.lib")

std::once_flag swapBuffersInit;

class Vector3 {
public:
    float x;
    float y;
    float z;
};

typedef BOOL(__stdcall* twglSwapBuffers)(_In_ HDC hDc);
twglSwapBuffers owglSwapBuffers;

typedef BOOL(WINAPI* hk_SetCursorPos)(int, int);
hk_SetCursorPos ocursor;

HWND gWnd;

bool drawMenu = false;

void hidecursor();
void explosionPatch(int explosionMultiplier, uintptr_t moduleBase);
void pewpewPatch(int pewMultiplier, uintptr_t moduleBase);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oWndProc;


bool needToNopMovement = true;
bool needToPatchMovement = true;

LRESULT APIENTRY hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_KEYDOWN:
            if (wParam == VK_F1 || wParam == VK_INSERT)
            {
                drawMenu = !drawMenu;
                if (drawMenu) {
                    needToNopMovement = true;
                    needToPatchMovement = true;
                }
                return true;
            }
    }

    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    if (drawMenu)
    {
        return true;
    }

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void onSwapBuffersInit()
{
    glewInit(); // initialize glew
    ImGui::CreateContext(); // create ImGui's context
    ImGui_ImplWin32_Init(gWnd);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

//handles only ctivating a function on keyDown [uparrow, downarrow, rightarrow, q, i, k, space, leftarrow, v]
bool kpHandler[10] = { true, true, true, true, true, true, true, true, true, true };

//handles if a cheat is enabled / disabled
bool prevCheatHandler[12] = { false, false, false, false, false, false, false, false , false , false , false , false };
bool cheatHandler[12]     = { false, false, false, false, false, false, false, false , false , false , false , false };

float saved_x = 0;
float saved_y = 0;
float saved_z = 0;

float setVelo = 0;

float savedX = 0.00;
float savedY = 0.00;
float savedZ = 0.00;

int desiredSpeed = 1;
int desiredGameSpeedMultiple = 9;
float desitedGameSpeed = BASE_GAME_SPEED;

int prevExplSize = 0;
int xplSize = 0;

int prevPewSize = 0;
int pewSize = 0;

bool hwCursor(int x, int y) {

    std::cout << "Cursor hook" << std::endl;

    if (drawMenu) {
        return false;
    }

    return ocursor(x, y);
}

//graphical debug stuff
bool drawBoundsRef = false;
bool drawBodiesRef = false;
bool drawShadowVolRef = false;

bool hwglSwapBuffers(_In_ HDC hDc)
{
    std::cout << "Swap hook" << std::endl;
    HANDLE mainHandle = GetModuleHandle(L"teardown.exe");
    uintptr_t moduleBase = (uintptr_t)mainHandle;
    uintptr_t game = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x0 });
    uintptr_t player = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0xA0, 0x0 });
    uintptr_t renderer = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x38, 0x0 });
    uintptr_t scene = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x40, 0x0 });

    //misc player values
    float* x = (float*)(player);
    float* y = (float*)(player + 4);
    float* z = (float*)(player + 8);
    float* yVelo = (float*)(player + 0x38 + 4);
    float* pHealth = (float*)(player + 0x015C);
    float* pSpeed = (float*)(player + 0x0160);
    bool* isPaused = (bool*)(game + 0x0138);
    Vector3 cpos = Vector3();

    //handling flight
    float* vx = (float*)(player + 0x007C);
    float* vy = (float*)(player + 0x007C + 4);
    float* vz = (float*)(player + 0x007C + 8);

    float* qx = (float*)(player + 0x0088);
    float* qy = (float*)(player + 0x0088 + 4);
    float* qz = (float*)(player + 0x0088 + 8);
    float* qw = (float*)(player + 0x0088 + 12);

    float* fMov = (float*)(player + 0x00D4);
    float* fMovSide = (float*)(player + 0x00D4 + 4);

    //game vars and stuff
    float* gamespeed = (float*)(game + 0x144);

    byte campos[84] = {};
    byte camang[84] = {};

    std::call_once(swapBuffersInit, onSwapBuffersInit);

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

    //V
    if (((GetAsyncKeyState(0x56) >> 15) & 0x0001) == 0x0001) {
        if (kpHandler[9]) {
            cheatHandler[8] = !cheatHandler[8];
             kpHandler[9] = false;
        }
    }
    else {
        kpHandler[9] = true;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuiIO* IO = &ImGui::GetIO();
    IO->MouseDrawCursor = drawMenu;

    //MENU GOES HERE
    if (drawMenu) {
        if (needToNopMovement) {
            mem::Nop((byte*)(moduleBase + 0xA9778), 8);
            mem::Nop((byte*)(moduleBase + 0xA9780), 8);
            needToNopMovement = false;
        }
        *fMov = 0.0f;
        *fMovSide = 0.0f;

        //*(bool*)(game + 0x0138) = true;
        ImGui::Begin("TD-Internal");
        ImGui::Checkbox("Godmode", &cheatHandler[0]);
        ImGui::Checkbox("Slow motion [Q]", &cheatHandler[1]);
        ImGui::SameLine();
        ImGui::PushItemWidth(200);
        ImGui::SliderInt("Game speed", &desiredGameSpeedMultiple, 1, 15);
        ImGui::Checkbox("Jetpack", &cheatHandler[2]);
        ImGui::Checkbox("Speedhack      ", &cheatHandler[3]);
        ImGui::SameLine();
        ImGui::PushItemWidth(200);
        ImGui::SliderInt("Run speed", &desiredSpeed, 1, 15);
        ImGui::Checkbox("No walls", &cheatHandler[4]);
        ImGui::Checkbox("Explosion boost", &cheatHandler[5]);
        ImGui::SameLine();
        ImGui::PushItemWidth(200);
        ImGui::SliderInt("Explosion size", &xplSize, 1, 15);
        ImGui::Checkbox("Gun boost      ", &cheatHandler[6]);
        ImGui::SameLine();
        ImGui::PushItemWidth(200);
        ImGui::SliderInt("Gun size", &pewSize, 1, 15);
        ImGui::Checkbox("No use cooldown", &cheatHandler[7]);
        ImGui::Checkbox("Freecam [V]", &cheatHandler[8]);
        ImGui::Checkbox("Show boundaries", &drawBoundsRef);
        ImGui::Checkbox("Show bodies", &drawBodiesRef);
        ImGui::Checkbox("Depth map", &drawShadowVolRef);
        ImGui::BeginGroup();

        std::stringstream gmss;
        gmss << "Game: 0x" << std::hex << game;
        std::string strgm = gmss.str();

        std::stringstream plss;
        plss << "Player: 0x" << std::hex << player;
        std::string strpl = plss.str();

        std::stringstream scss;
        scss << "Scene: 0x" << std::hex << scene;
        std::string strsc = scss.str();

        std::stringstream ress;
        ress << "Renderer: 0x" << std::hex << renderer;
        std::string strre = ress.str();

        std::stringstream bess;
        bess << "Base: 0x" << std::hex << moduleBase;
        std::string strbe = bess.str();

        ImGui::Text(strgm.c_str());
        ImGui::Text(strpl.c_str());
        ImGui::Text(strsc.c_str());
        ImGui::Text(strre.c_str());
        ImGui::Text(strbe.c_str());
        ImGui::EndGroup();
        ImGui::End();
    }
    else {
        if (needToPatchMovement) {
            mem::Patch((byte*)(moduleBase + 0xA9778), (byte*)"\xF3\x0F\x11\xB7\xD4\x00\x00\x00", 8);
            mem::Patch((byte*)(moduleBase + 0xA9780), (byte*)"\xF3\x0F\x11\xBF\xD8\x00\x00\x00", 8);
            needToPatchMovement = false;
        }
    }

    if (drawBoundsRef) {
        *(byte*)(renderer + 0x7A60) = 0x01;
    }
    else {
        *(byte*)(renderer + 0x7A60) = 0x00;
    }

    if (drawBodiesRef) {
        *(byte*)(renderer + 0x7A61) = 0x01;
    }
    else {
        *(byte*)(renderer + 0x7A61) = 0x00;
    }

    if (drawShadowVolRef) {
        *(byte*)(renderer + 0x7A63) = 0x01;
    }
    else {
        *(byte*)(renderer + 0x7A63) = 0x00;
    }

    if (cheatHandler[0] != prevCheatHandler[0]) {
        if (cheatHandler[0]) {
            //enable godmode
            mem::Nop((byte*)(moduleBase + 0xA6182), 8); //stop setting health value
            mem::Nop((byte*)(moduleBase + 0xA8CF3), 8); // basic damage
            mem::Nop((byte*)(moduleBase + 0xA7FB7), 8); // fall damage
            mem::Nop((byte*)(moduleBase + 0xA9E95), 8); // fire damage
            mem::Nop((byte*)(moduleBase + 0xA9F18), 8); // drowning damage
            *pHealth = 1.0f; //set health to full
        }
        else {
            //disable godmode
            mem::Patch((byte*)(moduleBase + 0xA6182), (byte*)"\xF3\x0F\x11\x81\x5C\x01\x00\x00", 8); // start setting health value
            mem::Patch((byte*)(moduleBase + 0xA8CF3), (byte*)"\xF3\x0F\x11\x91\x5C\x01\x00\x00", 8); // basic damage
            mem::Patch((byte*)(moduleBase + 0xA7FB7), (byte*)"\xF3\x0F\x11\x86\x5C\x01\x00\x00", 8); // fall damage
            mem::Patch((byte*)(moduleBase + 0xA9E95), (byte*)"\xF3\x0F\x11\x87\x5C\x01\x00\x00", 8); // fire damage
            mem::Patch((byte*)(moduleBase + 0xA9F18), (byte*)"\xF3\x0F\x11\x87\x5C\x01\x00\x00", 8); // drowning damage
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

            explosionPatch(xplSize, moduleBase);
        }
        else {
            //disable bigger explosions
            mem::Patch((byte*)(moduleBase + 0xC40DB), (byte*)"\xF3\x41\x0F\x59\xF1\xF3\x41\x0F\x5F\xF5\xF3\x41\x0F\x5D\xF3", 15); //unpatch 
            mem::Patch((byte*)(moduleBase + 0xA508C), (byte*)"\xF3\x0F\x59\xF7\xF3\x41\x0F\x5F\xF0\xF3\x41\x0F\x5D\xF4\xF3\x0F\x59\xF7\xF3\x0F\x58\x35\xA2\xAC\x24\x00", 18); //unpatch 
        }
    }

    if (cheatHandler[5] && xplSize != prevExplSize) {
        explosionPatch(xplSize, moduleBase);
        prevExplSize = xplSize;
    }

    if (cheatHandler[6] != prevCheatHandler[6]) {
        if (cheatHandler[6]) {
            //enable bigger guns
            pewpewPatch(pewSize, moduleBase);
        }
        else {
            //disable bigger guns
            mem::Patch((byte*)(moduleBase + 0xC3C88), (byte*)"\x0F\x28\xF8\xF3\x41\x0F\x5C\xFB\xF3\x41\x0F\x59\xF9\xF3\x41\x0F\x5F\xFD\xF3\x41\x0F\x5D\xFB", 23); //unpatch 
        }
    }

    if (cheatHandler[6] && pewSize != prevPewSize) {
        pewpewPatch(pewSize, moduleBase);
        prevPewSize = pewSize;
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

    if (cheatHandler[8] != prevCheatHandler[8]) {
        if (cheatHandler[8]) {
            //enable fly

            //nop instructions for camera movement
            mem::Nop((byte*)(moduleBase + 0xA6621), 5);
            mem::Nop((byte*)(moduleBase + 0xA6626), 8);
            mem::Nop((byte*)(moduleBase + 0xA665F), 8);

            saved_x = *x;
            saved_y = *y + 20;
            saved_x = *z;
        }
        else {
            //disable fly
            mem::Patch((byte*)(moduleBase + 0xA6621), (byte*)"\xF3\x0F\x11\x43\x7C", 5); //OK
            mem::Patch((byte*)(moduleBase + 0xA6626), (byte*)"\xF3\x0F\x11\x8B\x84\x00\x00\x00", 8); //OK
            mem::Patch((byte*)(moduleBase + 0xA665F), (byte*)"\xF3\x0F\x11\x93\x80\x00\x00\x00", 8); //OK

            *x = *vx;
            *y = *vy - 1.7f;
            *z = *vz;
        }
    }

    if (cheatHandler[8]) {

        //forward vector
        float nvX = 2 * (*qx * *qz + *qw * *qy);
        float nvY = 2 * (*qy * *qz - *qw * *qx);
        float nvZ = 1 - 2 * (*qx * *qx + *qy * *qy);

        float nvXl = 1 - 2 * (*qy * *qy + *qz * *qz);
        float nvYl = 2 * (*qx * *qy + *qw * *qz);
        float nvZl = 2 * (*qx * *qz - *qw * *qy);

        if (*fMov == 1) {
            *vx -= (nvX * 0.3);
            *vy -= (nvY * 0.3);
            *vz -= (nvZ * 0.3);
        }
        else if (*fMov == -1) {
            *vx += (nvX * 0.3);
            *vy += (nvY * 0.3);
            *vz += (nvZ * 0.3);
        }

        if (*fMovSide == 1) {
            *vx += (nvXl * 0.3);
            *vy += (nvYl * 0.3);
            *vz += (nvZl * 0.3);
        }
        else if (*fMovSide == -1) {
            *vx -= (nvXl * 0.3);
            *vy -= (nvYl * 0.3);
            *vz -= (nvZl * 0.3);
        }

        *x = saved_x;
        *y = saved_y;
        *z = saved_z;
        *yVelo = 0;
    }

    memcpy(prevCheatHandler, cheatHandler, sizeof cheatHandler);

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return owglSwapBuffers(hDc);
}

void hookMouse() {
    HMODULE u32 = GetModuleHandle(L"user32.dll");
    ocursor = (hk_SetCursorPos)GetProcAddress(u32, "SetCursorPos");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)ocursor, hwCursor);
    DetourTransactionCommit();
}

void HookGL()
{
    HMODULE OpenGL = GetModuleHandle(L"opengl32.dll");
    owglSwapBuffers = (twglSwapBuffers)GetProcAddress(OpenGL, "wglSwapBuffers");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)owglSwapBuffers, hwglSwapBuffers);
    DetourTransactionCommit();
}

void explosionPatch(int explosionMultiplier, uintptr_t moduleBase) {
    float finStrength = (5 + (2 * explosionMultiplier));
    unsigned char const* p = reinterpret_cast<unsigned char const*>(&finStrength);
    byte bytecode_rocket[15] = { 0xBA, p[0], p[1], p[2], p[3], 0x66, 0x0F, 0x6E, 0xF2, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }; // (mov edx,[finStrength]) , (movd xmm6,edx)
    byte bytecode_bomb[26] = { 0xBA, p[0], p[1], p[2], p[3], 0x66, 0x0F, 0x6E, 0xF2, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }; // (mov edx,[finStrength]) , (movd xmm6,edx)
    mem::Patch((byte*)(moduleBase + 0xC40DB), (byte*)bytecode_rocket, 15);
    mem::Patch((byte*)(moduleBase + 0xA508C), (byte*)bytecode_bomb, 18);
}

void pewpewPatch(int pewMultiplier, uintptr_t moduleBase) {
    float finStrength = (5 + (2 * pewMultiplier));
    unsigned char const* p = reinterpret_cast<unsigned char const*>(&finStrength);
    byte bytecode_gun[23] = { 0xBA, p[0], p[1], p[2], p[3], 0x66, 0x0F, 0x6E, 0xFA, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }; // (mov edx,[finStrength]) , (movd xmm6,edx)
    mem::Patch((byte*)(moduleBase + 0xC3C88), (byte*)bytecode_gun, 23);
}

DWORD WINAPI main(HMODULE hModule)
{
    //number of cheats for menu
    int CHEAT_COUNT = 8;

    //make console window and enable stdout
    //int selectedIndex = 0;
    //AllocConsole();
    //std::wstring strW = L"Internal diddler";
    //SetConsoleTitle(strW.c_str());
    //HWND console = GetConsoleWindow();
    //DeleteMenu(GetSystemMenu(console, false), SC_CLOSE, MF_BYCOMMAND);
    //SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    //FILE* f;
    //freopen_s(&f, "CONOUT$", "w", stdout);
    //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //hidecursor();
    //std::cout.precision(2);
    //SMALL_RECT tmp = { 0, 0, 120, 15 };
    //SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &tmp);

    MessageBox(0, L"Injection complete.\nInsert or F1 for menu", L"Notice", MB_ICONINFORMATION);

    HANDLE mainHandle = GetModuleHandle(L"teardown.exe");
    uintptr_t moduleBase = (uintptr_t)mainHandle;
    uintptr_t game = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x0 });
    uintptr_t player = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0xA0, 0x0 });
    uintptr_t renderer = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x38, 0x0 });
    uintptr_t scene = mem::FindDMAAddy(moduleBase + 0x003E4520, { 0x40, 0x0 });

    //opengl stuff by nyhu
    gWnd = FindWindow(0, L"Teardown");
    HookGL();
    hookMouse();
    oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(gWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hWndProc)));

    //patch basic instructions
    mem::Patch((byte*)(moduleBase + 0xA618a), (byte*)"\xF3\x0F\x10\x81\x60\x01\x00\x00", 8); //health value patch
    mem::Nop((byte*)(moduleBase + 0x242D8), 10); //stop setting gamespeed
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




