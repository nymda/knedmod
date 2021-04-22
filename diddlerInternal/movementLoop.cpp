#include "TDFuncs.h"
#include "types.h"
#include "Global.h"
#include "noclip.h"
#include "lantern.h"

void hkMov(float* a1, float* a2, DWORD* a3) {
    //this runs multiple times per frame, can be used as a tick reference?
    //oCreateLight works when called here
    //noclip does not work, player is not teleported to 000,500,000

    //td::Vec3 col = { 1.f, 1.f, 1.f };
    //glb::oCreateLight(glb::renderer, &glb::player->position, &col, 0.1f, 0.1f, 1.5f, 1.5f);

    return glb::oMovLoop(a1, a2, a3);
}

void terminateMovementHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::oMovLoop, hkMov);
    DetourTransactionCommit();
}

void initMovementHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::oMovLoop, hkMov);
    DetourTransactionCommit();
}
