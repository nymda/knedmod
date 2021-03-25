#include "TDFuncs.h"
#include "types.h"
#include "Global.h"

uintptr_t hkItem(td::small_string& path, float a2, float scale) {

    std::cout << path.c_str() << " " << a2 << " " << scale << std::endl;

    return glb::oSpawnVox(path, a2, scale);
}

void initTestHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::oSpawnVox, hkItem);
    DetourTransactionCommit();
}

void terminateTestHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::oSpawnVox, hkItem);
    DetourTransactionCommit();
}