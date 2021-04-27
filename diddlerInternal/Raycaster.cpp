#include "types.h"
#include "Global.h"
#include "Raycaster.h"

namespace raycaster {

    rayData castRayManual(td::Vec3 position, td::Vec3 rotation, RaycastFilter* filterCus) {

        td::Vec3 pCopy = position;
        td::Vec3 rCopy = rotation;

        filterCus->m_Mask = -1;
        td::Vec3 output{};
        uintptr_t oShape;
        float outDist = 0.f;
        glb::oRC(glb::scene, &pCopy, &rCopy, 250.f, filterCus, &outDist, &output, &oShape, nullptr);
        if (outDist == 0) {
            outDist = 1000.f;
        }

        td::Vec4 worldPos = { pCopy.x + (rCopy.x * outDist),  pCopy.y + (rCopy.y * outDist),  pCopy.z + (rCopy.z * outDist), outDist };
        td::Vec3 worldPosV3 = { worldPos.x, worldPos.y, worldPos.z };
        rayData rdp;
        rdp.distance = outDist;
        rdp.worldPos = worldPosV3;
        rdp.angle = output;
        rdp.hitShape = (TDShape*)oShape;
        return rdp;
    }

    rayData castRayPlayer() {
        td::Vec3 euler = glb::player->cameraEuler();
        td::Vec3 camPos = glb::player->cameraPosition;

        RaycastFilter filter{ 0 };
        filter.m_Mask = -1;
        td::Vec3 output{};
        uintptr_t oShape;
        float outDist = 0.f;
        glb::oRC(glb::scene, &camPos, &euler, 250.f, &filter, &outDist, &output, &oShape, nullptr);
        if (outDist == 0) {
            outDist = 1000.f;
        }

        td::Vec4 worldPos = { camPos.x + (euler.x * outDist),  camPos.y + (euler.y * outDist),  camPos.z + (euler.z * outDist), outDist };
        td::Vec3 worldPosV3 = { worldPos.x, worldPos.y, worldPos.z };
        rayData rdp;
        rdp.distance = outDist;
        rdp.worldPos = worldPosV3;
        rdp.angle = output;
        rdp.hitShape = (TDShape*)oShape;
        return rdp;
    }
}

