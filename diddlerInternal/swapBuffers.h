#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#include <WinUser.h>

void initHIDsHook();
void initSwapBuffersHook();
void terminateHIDsHook();
void terminateSwapBuffersHook();