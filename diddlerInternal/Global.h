#pragma once
#include "types.h"
#include "mem.h"
#include "glew.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <ctime>
#include "detours.h"
#include <mutex>
#include "global.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <random>
#include "TDObjects.h"



struct RaycastFilter
{
    int32_t m_Mask;
    int32_t m_Allowed;
    int32_t m_Ignored;
    bool m_RejectTransparent;
    uint8_t m_Pad[3];
    td::small_vector<uintptr_t> m_IgnoredBodies;
    uintptr_t m_IgnoredBodiesMemory[4];
    td::small_vector<uintptr_t> m_IgnoredShapes;
};

typedef BOOL(WINAPI* hk_SetCursorPos)(int, int);
typedef BOOL(__stdcall* twglSwapBuffers)(_In_ HDC hDc);
typedef void (*tPaint) (uintptr_t* Scene, td::Vec3* Position, float size, int darken, float dispersion);
typedef void (*tFire) (uintptr_t* Scene, td::Vec3* Position);
typedef uintptr_t(__fastcall* tSpawnVox) (td::small_string* path, TDScene* scene, float scale);
typedef char(__fastcall* idfk) (__int64 a1, __int64 a2, signed int* a3, signed int* a4, signed int* a5);

typedef void(__fastcall* damagePlayer) (TDPlayer* player, float damage);
typedef void(__fastcall* CreateTexture) (uintptr_t ptr);
typedef void(__fastcall* CreatePhysics) (uintptr_t ptr);
typedef void(__fastcall* UpdateShapes) (uintptr_t ptr);
typedef void(__fastcall* B_Constructor) (uintptr_t ptr, uintptr_t parent);
typedef uintptr_t(__fastcall* S_Constructor) (uintptr_t ptr, uintptr_t parent);
typedef void(__fastcall* SetDynamic) (uintptr_t ptr, bool dynamic);
typedef uintptr_t(__fastcall* TMalloc)(size_t);
typedef void(__fastcall* TFree)(uintptr_t mem);
typedef void(__fastcall* frameDrawLine)(TDRenderer* renderer, const td::Vec3& p1, const td::Vec3& p2, const td::Color& c1, const td::Color& c2, bool use_depth);
typedef void(__fastcall* rayCast)(TDScene* scene, td::Vec3* pos, td::Vec3* rot, float dist, RaycastFilter* filter, float* outDist, td::Vec3* out, uintptr_t* out_shape, uintptr_t* out_palette);
typedef void(__fastcall* damageStuff)(uintptr_t scene, td::Vec3* pos); //broken 
typedef void(__fastcall* spawnFire)(uintptr_t scene, td::Vec3* pos);
typedef void(__fastcall* pewpew)(uintptr_t scene, td::Vec3* pos, td::Vec3* dir, INT32 type);

typedef void(__fastcall* cameraPositioning)(int a1, int a2, int a3, int a4);
typedef void(__fastcall* modApiLinker)(__int64 a1);
typedef int(__fastcall* makeHole)(byte data1[32], byte data2[32], byte data3[32], byte data4[32]);
typedef void(__fastcall* createExplosion)(uintptr_t scene, td::Vec3* pos, float radius);
typedef uintptr_t(__fastcall* spawnParticle)(DWORD* un1, INT64 un2, INT64 un3, INT64 un4, int un5, UINT un6);

typedef void(__stdcall* damageObject)(__int64 a1, __int64* a2, float* a3, int a4, float a5, int a6, int a7, char a8, signed int* a9);
typedef void(__stdcall* createExplosionWrapped)(double unkn, td::Vec3* pos, float power);
typedef void(__fastcall* spawnParticleWrapped)(double a1, __int64 a2);

//SEE TDFUNCS.CPP FOR SIGSCANNING
namespace glb {
    extern TDObjectList* TDOL;
    extern TDPlayer* player;
    extern TDGame* game;
    extern TDRenderer* renderer;
    extern TDScene* scene;
    
    extern cameraPositioning oCamPos;
    extern damageObject oDamageObject;
    extern modApiLinker oMAL;
    extern makeHole oMakeHole;
    extern damagePlayer oDamagePlayer;
    extern idfk killme;
    extern tPaint oPaint;
    extern tFire oFire;
    extern tSpawnVox oSpawnVox;
    extern CreateTexture oCreateTexture;
    extern CreatePhysics oCreatePhysics;
    extern UpdateShapes oUpdateShapes;
    extern B_Constructor oB_Constructor;
    extern S_Constructor oS_Constructor;
    extern SetDynamic oSetDynamic;
    extern TMalloc oTMalloc;
    extern frameDrawLine oFDL;
    extern rayCast oRC;
    extern damageStuff oDamageStuff;
    extern spawnFire oSpawnFire;
    extern pewpew oPewpew;

    extern createExplosion TDcreateExplosion;
    extern spawnParticle TDspawnParticle;

    extern createExplosionWrapped TDcreateExplosionWrapped;
    extern spawnParticleWrapped TDspawnParticleWrapped;

    extern uintptr_t moduleBase;
    extern twglSwapBuffers owglSwapBuffers;
    extern bool displayMenu;
    extern HWND gWnd;
    extern hk_SetCursorPos ocursor;
    extern HMODULE hMdl;
}
