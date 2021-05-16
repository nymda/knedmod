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

void hkDoDamage(uintptr_t a1, uintptr_t a2, td::Vec3* a3, float a4, float a5, uintptr_t a6, uintptr_t a7, uintptr_t a8) {
  
    //uintptr_t sceneSpecial = mem::FindDMAAddy(((uintptr_t)glb::scene + 0xA8), { 0x16 });

    uintptr_t sceneSpecial = *(uintptr_t*)((uintptr_t)glb::scene + 16);

    //std::cout << a1 << " : " << sceneSpecial << std::endl;
    //std::cout << a2  << std::endl;

    float damageP1 = 10.f;
    float damageP2 = 10.f;

    std::cout << "A1 " << a1 << " : " << sceneSpecial << std::endl;
    std::cout << "A2 " << a2 << std::endl;
    std::cout << "A3 " << a3 << std::endl;
    std::cout << "A4 " << a4 << std::endl;
    std::cout << "A5 " << a5 << std::endl;
    std::cout << "A6 " << a6 << std::endl;
    std::cout << "A7 " << a7 << std::endl;
    std::cout << "A8 " << a8 << std::endl;
    std::cout << "=========" << std::endl;

    //arg 1 : special pointer derived from scene.
    //arg 2 : unknwon pointer, can be null. may be parent object?
    //arg 3 : vec3 pointer, position
    //arg 4 : damage to SOFT objects
    //arg 5 : unknown float, may be related to power

    return glb::oDamageObject(sceneSpecial, a2, a3, damageP1, damageP2, a6, a7, a8);
}

uintptr_t structContainer;
void hkCreateParticle(uintptr_t* ptr, td::particleInfo& orig, td::Vec3& position, td::Vec3& velocity, float lifetime) {
    uintptr_t special = *(uintptr_t*)((uintptr_t)glb::scene + 0x68);

    if (!structContainer) {
        structContainer = (uintptr_t)malloc(256);
    }

    memcpy((void*)structContainer, (void*)&orig, 256);
    std::cout << "InfoStruct: " << structContainer << std::endl;
    std::cout << "Position  : X:" << std::to_string(position.x) << " Y: " << std::to_string(position.y) << " Z: " << std::to_string(position.z) << std::endl;
    std::cout << "Velocity  : X:" << std::to_string(velocity.x) << " Y: " << std::to_string(velocity.y) << " Z: " << std::to_string(velocity.z) << std::endl;
    std::cout << "Lifetime  : " << lifetime << std::endl;
    std::cout << "==========" << std::endl;

    std::cout << std::to_string(orig.U01) << std::endl;
    std::cout << std::to_string(orig.U02) << std::endl;
    std::cout << std::to_string(orig.U03) << std::endl;
    std::cout << std::to_string(orig.U04) << std::endl;
    std::cout << std::to_string(orig.U05) << std::endl;
    std::cout << std::to_string(orig.U06) << std::endl;
    std::cout << std::to_string(orig.U07) << std::endl;
    std::cout << std::to_string(orig.U08) << std::endl;
    std::cout << std::to_string(orig.U09) << std::endl;
    std::cout << std::to_string(orig.U10) << std::endl;
    std::cout << std::to_string(orig.U11) << std::endl;
    std::cout << std::to_string(orig.U12) << std::endl;
    std::cout << std::to_string(orig.U13) << std::endl;
    std::cout << std::to_string(orig.U14) << std::endl;
    std::cout << std::to_string(orig.U15) << std::endl;
    std::cout << std::to_string(orig.U16) << std::endl;
    std::cout << std::to_string(orig.U17) << std::endl;
    std::cout << std::to_string(orig.U18) << std::endl;
    std::cout << std::to_string(orig.U19) << std::endl;
    std::cout << std::to_string(orig.U20) << std::endl;
    std::cout << std::to_string(orig.U21) << std::endl;
    std::cout << std::to_string(orig.U22) << std::endl;
    std::cout << std::to_string(orig.U23) << std::endl;
    std::cout << std::to_string(orig.U24) << std::endl;
    std::cout << std::to_string(orig.U25) << std::endl;
    std::cout << std::to_string(orig.U26) << std::endl;
    std::cout << std::to_string(orig.U27) << std::endl;
    std::cout << std::to_string(orig.U28) << std::endl;
    std::cout << std::to_string(orig.U29) << std::endl;
    std::cout << std::to_string(orig.U30) << std::endl;
    std::cout << std::to_string(orig.U31) << std::endl;
    std::cout << std::to_string(orig.U32) << std::endl;
    std::cout << std::to_string(orig.U33) << std::endl;
    std::cout << std::to_string(orig.U34) << std::endl;
    std::cout << std::to_string(orig.U35) << std::endl;
    std::cout << std::to_string(orig.U36) << std::endl;
    std::cout << std::to_string(orig.U37) << std::endl;
    std::cout << std::to_string(orig.U38) << std::endl;
    std::cout << std::to_string(orig.U39) << std::endl;
    std::cout << std::to_string(orig.U40) << std::endl;
    std::cout << std::to_string(orig.U41) << std::endl;
    std::cout << std::to_string(orig.U42) << std::endl;
    std::cout << std::to_string(orig.U43) << std::endl;
    std::cout << std::to_string(orig.U44) << std::endl;
    std::cout << std::to_string(orig.U45) << std::endl;
    std::cout << std::to_string(orig.U46) << std::endl;
    std::cout << std::to_string(orig.U47) << std::endl;
    std::cout << std::to_string(orig.U48) << std::endl;
    std::cout << std::to_string(orig.U49) << std::endl;
    std::cout << std::to_string(orig.U50) << std::endl;
    std::cout << std::to_string(orig.U51) << std::endl;
    std::cout << std::to_string(orig.U52) << std::endl;
    std::cout << std::to_string(orig.U53) << std::endl;
    std::cout << std::to_string(orig.U54) << std::endl;
    std::cout << std::to_string(orig.U55) << std::endl;
    std::cout << std::to_string(orig.U56) << std::endl;
    std::cout << std::to_string(orig.U57) << std::endl;
    std::cout << std::to_string(orig.U58) << std::endl;
    std::cout << std::to_string(orig.U59) << std::endl;
    std::cout << std::to_string(orig.U60) << std::endl;
    std::cout << std::to_string(orig.U61) << std::endl;
    std::cout << std::to_string(orig.U62) << std::endl;
    std::cout << std::to_string(orig.U63) << std::endl;
    std::cout << std::to_string(orig.U64) << std::endl;

    td::particleInfo testing = { };
    testing.U01 = orig.U01;
    testing.U02 = orig.U02;
    testing.U03 = orig.U03;
    testing.U04 = orig.U04;      //R
    testing.U05 = orig.U05;      //G
    testing.U06 = orig.U06;      //B
    testing.U07 = orig.U07;      //A
    testing.U08 = orig.U08;
    testing.U09 = orig.U09;
    testing.U10 = orig.U10;
    testing.U11 = orig.U11;
    testing.U12 = orig.U12;
    testing.U13 = orig.U13;
    testing.U14 = orig.U14;
    testing.U15 = orig.U15;
    testing.U16 = orig.U16;
    testing.U17 = orig.U17;
    testing.U18 = orig.U18;
    testing.U19 = orig.U19;
    testing.U20 = orig.U20;
    testing.U21 = orig.U21;
    testing.U22 = orig.U22;
    testing.U23 = orig.U23;
    testing.U24 = orig.U24;
    testing.U25 = orig.U25;
    testing.U26 = orig.U26;
    testing.U27 = orig.U27;
    testing.U28 = orig.U28;
    testing.U29 = orig.U29;
    testing.U30 = orig.U30;
    testing.U31 = orig.U31;
    testing.U32 = orig.U32;
    testing.U33 = orig.U33;
    testing.U34 = orig.U34;
    testing.U35 = orig.U35;
    testing.U36 = orig.U36;
    testing.U37 = orig.U37;
    testing.U38 = orig.U38;
    testing.U39 = orig.U39;
    testing.U40 = orig.U40;
    testing.U41 = orig.U41;
    testing.U42 = orig.U42;
    testing.U43 = orig.U43;
    testing.U44 = orig.U44;
    testing.U45 = orig.U45;
    testing.U46 = orig.U46;
    testing.U47 = orig.U47;
    testing.U48 = orig.U48;
    testing.U49 = orig.U49;
    testing.U50 = orig.U50;
    testing.U51 = orig.U51;
    testing.U52 = orig.U52;
    testing.U53 = orig.U53;
    testing.U54 = orig.U54;
    testing.U55 = orig.U55;
    testing.U56 = orig.U56;
    testing.U57 = orig.U57;
    testing.U58 = orig.U58;
    testing.U59 = orig.U59;
    testing.U60 = orig.U60;
    testing.U61 = orig.U61;
    testing.U62 = orig.U62;
    testing.U63 = orig.U63;
    testing.U64 = orig.U64;

    glb::TDspawnParticle((DWORD*)special, (td::particleInfo*)&testing, position, velocity, lifetime);
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

void hkEnvUpdate(uintptr_t environment) {

    std::cout << "Env: " << environment << std::endl;

    return glb::oEnvUpdate(environment);
}

uintptr_t hkSpawnVox(td::small_string* path, td::small_string* subpath, float scale) {
    std::cout << "Path: " << path->c_str() << std::endl;
    std::cout << "SPath: " << subpath->c_str() << std::endl;
    std::cout << "==========" << std::endl;

    return glb::oSpawnVox(path, subpath, scale);
}

void hkSetAttribute(TDShape* object, td::small_string* a1, td::small_string* a2) {

    std::cout << "SOA | Object: " << object << " | A1: " << a1->c_str() << " | A2: " << a2->c_str() << std::endl;

    glb::oSOA(object, a1, a2);

    return;
}

void hkAttachJoint(void* a1, void* a2, void* a3, void* a4, void* a5) {

    std::cout << "a1: " << (uintptr_t)a1 << std::endl;
    std::cout << "a2: " << (uintptr_t)a2 << " (" << std::to_string(((td::Vec3*)a2)->x) << " : " << std::to_string(((td::Vec3*)a2)->y) << " : " << std::to_string(((td::Vec3*)a2)->z) << std::endl;
    std::cout << "a3: " << (uintptr_t)a3 << std::endl;
    std::cout << "a4: " << (uintptr_t)a4 << std::endl;
    std::cout << "a5: " << (uintptr_t)a5 << std::endl;
    std::cout << "=====" << std::endl;

    glb::oAttachJoint(a1, a2, a3, a4, a5);
}

void hkContextItem(char* a1, int a2, int a3, float* a4) {

    std::cout << std::string(a1) << std::endl;

    return glb::oAddCItem(a1, a2, a3, a4);
}

void initTestHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::oAttachJoint, hkAttachJoint);
    DetourTransactionCommit();
}

void terminateTestHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::oAttachJoint, hkAttachJoint);
    DetourTransactionCommit();
}