#include "TDFuncs.h"
#include "types.h"
#include "Global.h"
#include <cfloat>

std::vector<TDShape*> knownShapes = {};
bool init = true;
uintptr_t bufferCopy = 0;

struct particleInfoStruct {
    float R;
    float G;
    float B;
    float A;
    float FadeR;
    float FadeG;
    float FadeB;
    float FadeA;
    float U05;
    float U06;
    float StartSize;
    float EndSize;
    float U09;
    float U10;
    float U11;
    float U12;
};

void printB4(byte obj[4]) {
    if (obj) {
        std::cout << obj[0] << " : " << obj[1] << " : " << obj[2] << " : " << obj[3] << " : " << obj[4] << std::endl;
    }
    else {
        std::cout << "NULL OBJ" << std::endl;
    }
}

void hkDoDamage(uintptr_t a1, uintptr_t a2, td::Vec3* a3, float a4, float a5, uintptr_t a6, uintptr_t a7, uintptr_t a8, uintptr_t a9) {
  
    uintptr_t sceneSpecial = mem::FindDMAAddy(((uintptr_t)glb::scene + 0xA8), { 0x80 });
    //std::cout << a1 << " : " << sceneSpecial << std::endl;
    //std::cout << a2  << std::endl;

    float damageP1 = 10.f;
    float damageP2 = 10.f;

    std::cout << "A6 " << a6 << std::endl;
    std::cout << "A7 " << a7 << std::endl;
    std::cout << "A8 " << a8 << std::endl;
    std::cout << "A9 " << a9 << std::endl;
    std::cout << "=========" << std::endl;

    //arg 1 : special pointer derived from scene.
    //arg 2 : unknwon pointer, can be null. may be parent object?
    //arg 3 : vec3 pointer, position
    //arg 4 : damage to SOFT objects
    //arg 5 : unknown float, may be related to power

    return glb::oDamageObject(a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

void hkCreateParticle(uintptr_t* ptr, td::particleInfo& color, td::Vec3& position, td::Vec3& velocity, float lifetime, int type) {
    uintptr_t special = *(uintptr_t*)((uintptr_t)glb::scene + 0x68);
    //std::cout << "pointer: " << ptr << " pred: " << special << " lifetime: " << std::to_string(lifetime) << " type: " << std::to_string(type) << std::endl;
    
    //if (init) {
    //    bufferCopy = (uintptr_t)malloc(64);
    //    init = false;
    //}

    //memcpy((void*)bufferCopy, &color, 64);
    //std::cout << bufferCopy << std::endl;

    //particleInfoStruct tmp = *(particleInfoStruct*)color;

    //for (int i = 0; i < 128; ++i)
    //    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)buffer[i] << " ";
    //std::cout << "=====" << std::endl;

    td::Vec3 newVelo = { 0, 0, 0 };
    particleInfoStruct orig = *(particleInfoStruct*)&color;

    particleInfoStruct testing = { };
    //testing.R = 1.f;
    //testing.G = 1.f;
    //testing.B = 1.f;
    //testing.A = 1.f;
    //testing.FadeR = 1.f;
    //testing.FadeG = 0.f;
    //testing.FadeB = 0.f;
    //testing.FadeA = 1.f;
    //testing.U05 = orig.U05;
    //testing.U06 = orig.U06;
    //testing.StartSize = 0.1f;
    //testing.EndSize = 0.5f;
    //testing.U09 = orig.U09;
    //testing.U10 = orig.U10;
    //testing.U11 = orig.U11;
    //testing.U12 = orig.U12;

    testing.R = orig.R;
    testing.G = orig.G;
    testing.B = orig.B;
    testing.A = orig.A;
    testing.FadeR = orig.FadeR;
    testing.FadeG = orig.FadeG;
    testing.FadeB = orig.FadeB;
    testing.FadeA = orig.FadeA;
    testing.U05 = orig.U05;
    testing.U06 = orig.U06;
    testing.StartSize = orig.StartSize;
    testing.EndSize = orig.EndSize;
    testing.U09 = orig.U09;
    testing.U10 = orig.U10;
    testing.U11 = orig.U11;
    testing.U12 = orig.U12;

    glb::TDspawnParticle((DWORD*)special, (td::particleInfo*)&testing, position, velocity, lifetime, type);
}

bool isKnown(TDShape* test) {
    for (TDShape* current : knownShapes) {
        if (current == test) {
            return true;
        }
    }
    return false;
}

void hkCreateExplosion(uintptr_t scene, td::Vec3* pos, float power) {
    std::cout << "PTR: " << scene << std::endl;
    std::cout << "POS: " << pos << std::endl;
    std::cout << "PWR: " << std::to_string(power) << std::endl;

    return glb::TDcreateExplosion(scene, pos, power);
}

void hkItem(TDShape* shape, td::small_string* attribute, td::small_string* attribute2){
 
    //td::Vec3 high = { FLT_MAX, FLT_MAX, FLT_MAX };

    if (!isKnown(shape)) {
        knownShapes.push_back(shape);
        std::cout << "Shape: " << shape << "\nAttrib: " << attribute->c_str() << "\nAttrib2: " << attribute2->c_str() << std::endl;
        std::cout << "==========" << std::endl;
    }

    return glb::oSOA(shape, attribute, attribute2);
}

void hkCreateFire(TDScene* scene, td::Vec3* position) {
    uintptr_t sceneSpecial = *(uintptr_t*)((uintptr_t)glb::scene + 0x88);
    std::cout << "SCENE: " << scene << std::endl;
    std::cout << "TEST : " << sceneSpecial << std::endl;
}

void hkPointLight(TDRenderer* renderer, td::Vec3* a2, td::Vec3* a3, float a4, float a5, float a6, float a7, int a8) {
    std::cout << "PTR: " << renderer << std::endl;
    std::cout << "A2 : X: " << a2->x << " Y: " << a2->y << " Z: " << a2->z << std::endl;
    std::cout << "A3 : R: " << a3->x << " G: " << a3->y << " B: " << a3->z << std::endl;
    std::cout << "A4 : " << a4 << std::endl;
    std::cout << "A5 : " << a5 << std::endl;
    std::cout << "A6 : " << a6 << std::endl;
    std::cout << "A7 : " << a7 << std::endl;
    std::cout << "A8 : " << a8 << std::endl;
    std::cout << "=====" << std::endl;

    td::Vec3 col = { 1.f, 1.f, 1.f };

    return glb::oCreateLight(renderer, a2, a3, 0.1f, 0.1f, 1.5f, 1.5f);
}

void testMainHook(float* a1, float* a2, DWORD* a3) {

    td::Vec3 col = { 1.f, 1.f, 1.f };
    glb::oCreateLight(glb::renderer, &glb::player->position, &col, 0.1f, 0.1f, 1.5f, 1.5f);

    return glb::oMovLoop(a1, a2, a3);
}

void initTestHook() {
    return;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::oCreateLight, hkPointLight);
    //DetourAttach(&(PVOID&)glb::TDspawnParticle, hkCreateParticle);
    //DetourAttach(&(PVOID&)glb::oDamageObject, hkDoDamage);
    DetourTransactionCommit();
}

void terminateTestHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::oCreateLight, hkPointLight);
    //DetourDetach(&(PVOID&)glb::TDspawnParticle, hkCreateParticle);
    //DetourDetach(&(PVOID&)glb::oDamageObject, hkDoDamage);
    DetourTransactionCommit();
}