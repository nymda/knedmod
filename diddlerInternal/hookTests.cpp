#include "TDFuncs.h"
#include "types.h"
#include "Global.h"

uintptr_t hkItem(td::small_string* path, TDScene* scene, float scale) {

    std::cout << (*path).c_str() << " " << scene << " " << scale << std::endl;

    byte buffer[32] = {};
    memcpy(buffer, scene, 32);

    for (int i = 0; i < 32; ++i)
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)buffer[i] << " ";
    std::cout << std::endl;

    std::cout << *(float*)(scene + 4) << std::endl;
    std::cout << *(float*)(scene + 8) << std::endl;
    std::cout << std::hex << *(uintptr_t*)(scene + 16) << std::endl;

    return glb::oSpawnVox(path, scene, scale);
}

void dkDelItem(INT64 a1, INT64 a2, INT64 a3, INT64 a4, INT64 a5, INT64 a6, INT64 a7, INT64 a8) {

    std::cout << a1 << " " << a2  << " " << a3 << " " << a4 << " " << a5 << " " << a6 << " " << a7 << " " << a8 << std::endl;

    return glb::oTdDelBdy(a1, a2, a3, a4, a5, a6, a7, a8);
}

void initTestHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::oTdDelBdy, dkDelItem);
    DetourTransactionCommit();
}

void terminateTestHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::oTdDelBdy, dkDelItem);
    DetourTransactionCommit();
}