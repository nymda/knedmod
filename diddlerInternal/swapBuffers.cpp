#define GLEW_STATIC
#include "Global.h"
#include "TDObjects.h"
#include "swapBuffers.h"
#include "Raycaster.h"
#include "Mods.h"
#include "noclip.h"

#pragma comment(lib, "glew32s.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oWndProc;

std::once_flag swapBuffersInit;

LRESULT APIENTRY hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_F1 || wParam == VK_INSERT)
        {
            glb::displayMenu = !glb::displayMenu;
            return true;
        }

        if (wParam == mods::noclipKey) {
            noclip::ToggleNoclip();
        }
    }

    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    if (glb::displayMenu)
    {
        return true;
    }

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool hwCursor(int x, int y) {

    if (glb::displayMenu) {
        return false;
    }

    return glb::ocursor(x, y);
}

void onSwapBuffersInit()
{
    glewInit(); // initialize glew
    ImGui::CreateContext(); // create ImGui's context
    ImGui_ImplWin32_Init(glb::gWnd);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void initHIDsHook() {
    HMODULE u32 = GetModuleHandle(L"user32.dll");
    glb::ocursor = (hk_SetCursorPos)GetProcAddress(u32, "SetCursorPos");

    oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(glb::gWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hWndProc)));

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::ocursor, hwCursor);
    DetourTransactionCommit();
}

void terminateHIDsHook() {
    SetWindowLongPtrA(glb::gWnd, GWLP_WNDPROC, LONG_PTR(oWndProc));

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::ocursor, hwCursor);
    DetourTransactionCommit();
}

std::vector<spawner::LoadedSpawnableObject> spawnerObjects;
bool needToLoadObjects = true;

char position[64];
char velocity[64];
char cPosition[64];
char health[64];
static ImGuiTextFilter filter;

bool hwglSwapBuffers(_In_ HDC hDc)
{
    std::call_once(swapBuffersInit, onSwapBuffersInit);

    //if (((((GetAsyncKeyState(VK_F1) >> 15) & 0x0001) == 0x0001))) {
    //    glb::displayMenu = true;
    //}
    //else {
    //    glb::displayMenu = false;
    //}

    if (needToLoadObjects) {
        needToLoadObjects = false;
        spawnerObjects = spawner::enumerateSpawnableObjects();
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuiIO* IO = &ImGui::GetIO();
    IO->MouseDrawCursor = glb::displayMenu;

    if (glb::displayMenu) {
        ImGui::Begin("KnedMod");
        {
            int counter = 0;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("ChildL", ImVec2(ImGui::GetWindowWidth() * 0.75, 0), true, window_flags);

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Spawn menu"))
                {
                    ImGui::MenuItem("Refresh", NULL, &needToLoadObjects);
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::Text("Search: ");
            ImGui::SameLine();
            filter.Draw("##search", 210.f);

            for (spawner::LoadedSpawnableObject lso : spawnerObjects) {

                if (filter.PassFilter(lso.basePath.c_str())) {
                    if (counter % 5 != 0) {
                        ImGui::SameLine();
                    }

                    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                    ImGui::BeginChild(std::to_string(counter).c_str(), ImVec2(ImGui::GetWindowWidth() / 6, ImGui::GetWindowHeight() / 3), true, window_flags);

                    if (ImGui::BeginMenuBar())
                    {
                        if (ImGui::BeginMenu(lso.basePath.substr(4, lso.basePath.length() - 4).c_str()))
                        {
                            ImGui::EndMenu();
                        }
                        ImGui::EndMenuBar();
                    }

                    ImGui::Image((void*)(intptr_t)lso.imageTexture, ImVec2(ImGui::GetWindowWidth() - 17, ImGui::GetWindowWidth() - 17));

                    if (ImGui::Button("Spawn")) {
                        spawner::objectSpawnerParams osp;
                        spawner::spawnObjectProxy(lso.voxPath, osp);
                    }

                    if (ImGui::Button("Spawn w/ spawngun")) {
                        spawner::currentSpawngunObject = lso.voxPath;
                    }

                    ImGui::EndChild();
                    ImGui::PopStyleVar();

                    counter++;
                }       
            }

            ImGui::EndChild();
            ImGui::PopStyleVar();
        }

        ImGui::SameLine();

        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("ChildR", ImVec2(0, 0), true, window_flags);

            ImGui::Checkbox("Godmode", &mods::godmode);
            ImGui::Checkbox("Jetpack", &mods::jetpack);
            ImGui::Checkbox("Flamethrower", &mods::flamethrower);

            ImGui::EndChild();
            ImGui::PopStyleVar();
        }


        //ImGui::Text("UI test");
        //sprintf_s(health, 64, "HP: %.4f", glb::player->health);
        //ImGui::Text(health);
        //sprintf_s(position, 64, "X: %.4f Y: %.4f Z: %.4f", glb::player->position.x, glb::player->position.y, glb::player->position.z);
        //ImGui::Text(position);
        //ImGui::Text(glb::player->heldItemName);

        ImGui::End();
    }
 
    mods::execMods();

    //std::cout << glb::player->heldItemName << std::endl;

    //td::Vec4 ray = castRayPlayer();
    //std::cout << "X: " << std::to_string(ray.x) << " Y: " << std::to_string(ray.y) << " Z: " << std::to_string(ray.z) << " D: " << std::to_string(ray.w) << std::endl;

    //td::Vec3 boomPos = { ray.x, ray.y, ray.z };
    //glb::TDcreateExplosionWrapped(0, &boomPos, 5);
    //glb::createExplosion((uintptr_t)glb::scene, &boomPos, 5);

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return glb::owglSwapBuffers(hDc);
}

void terminateSwapBuffersHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::owglSwapBuffers, hwglSwapBuffers);
    DetourTransactionCommit();
}

void initSwapBuffersHook() {
    HMODULE OpenGL = GetModuleHandle(L"opengl32.dll");
    glb::owglSwapBuffers = (twglSwapBuffers)GetProcAddress(OpenGL, "wglSwapBuffers");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::owglSwapBuffers, hwglSwapBuffers);
    DetourTransactionCommit();
}

