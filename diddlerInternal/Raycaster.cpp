#include "types.h"
#include "Global.h"

td::Vec3 castRayManual() {

    return { 0, 0, 0 };
}

td::Vec4 castRayPlayer() {
    td::Vec3 euler = glb::player->cameraEuler();
    td::Vec3 camPos = glb::player->cameraPosition;

    RaycastFilter filter{ 0 };
    filter.m_Mask = -1;
    td::Vec3 output;
    uintptr_t oShape;
    float outDist = 0.f;
    glb::oRC(glb::scene, &camPos, &euler, 250.f, &filter, &outDist, &output, &oShape, nullptr);
    if (outDist == 0) {
        outDist = 1000.f;
    }

    return { camPos.x + (euler.x * outDist),  camPos.y + (euler.y * outDist),  camPos.z + (euler.z * outDist), outDist };
}