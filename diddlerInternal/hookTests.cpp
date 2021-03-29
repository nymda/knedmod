#include "TDFuncs.h"
#include "types.h"
#include "Global.h"

uintptr_t hkItem(td::small_string& path, float u1, float scale) {

    std::cout << path.c_str() << " " << u1 << " " << scale << std::endl;

    return 0;
}

void initTestHook() {
    //DetourTransactionBegin();
    //DetourUpdateThread(GetCurrentThread());
    //DetourAttach(&(PVOID&)glb::oSpawnVox, hkItem);
    //DetourTransactionCommit();
}

void terminateTestHook() {
    //DetourTransactionBegin();
    //DetourUpdateThread(GetCurrentThread());
    //DetourDetach(&(PVOID&)glb::oSpawnVox, hkItem);
    //DetourTransactionCommit();
}