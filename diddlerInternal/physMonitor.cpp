#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "drawCube.h"
#include "toolgun.h"
#include "camera.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "camera.h"
#include "dotProjector.h"
#include <glm/gtx/quaternion.hpp>

namespace physMonitor {
	spawner::spawnedObject monitor = {};
    TDScreen* currentScreen = 0;

    void spawnMonitor() {
        spawner::freeObjectSpawnParams osp = {};
        osp.useUserRotation = false;
        monitor = spawner::placeFreeObject("vox/Default/PVM/object.vox", osp);
        glb::setObjectAttribute(monitor.shapes[0], "nocull", "");

        currentScreen = (TDScreen*)glb::oTMalloc(3384);
        glb::tdConstructScreen(currentScreen, (uintptr_t)monitor.shapes[0]);
        currentScreen->m_Size = { 5, 5 };
        currentScreen->m_Resolution = { 640, 480 };
        td::small_string script_path = td::small_string("data/level/lee/script/tv.lua");
        td::small_string script_path_technical = td::small_string("RAW:data/level/lee/script/tv.lua");
        td::small_string script_path_type = td::small_string("");

        void* result = glb::oValidatePath(glb::game + 0xA8, &script_path, &script_path_technical, &script_path_type);

        currentScreen->m_Script = (td::small_string*)result;

        std::cout << "SCREEN: " << currentScreen << " SCR: " << result << std::endl;
        glb::tdUpdateScreen(currentScreen);
    }

}