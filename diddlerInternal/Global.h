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
#include "glm/glm.hpp"

struct RaycastFilter
{
    int32_t m_Mask;
    int32_t m_Allowed;
    int32_t m_Ignored;
    bool m_RejectTransparent;
    uint8_t m_Pad[3];
    td::small_vector<TDBody*> m_IgnoredBodies;
    TDBody* m_IgnoredBodiesMemory[4]; // small_vector should support pre-reserved buffers... not going to add that right now.
    td::small_vector<TDShape*> m_IgnoredShapes;
};

//world
typedef void(__fastcall* environmentUpdate)(uintptr_t env);
typedef void(__fastcall* rayCast)(TDScene* scene, td::Vec3* pos, td::Vec3* rot, float dist, RaycastFilter* filter, float* outDist, td::Vec3* out, uintptr_t* out_shape, uintptr_t* out_palette);
typedef void(__fastcall* frameDrawLine)(TDRenderer* renderer, const td::Vec3& p1, const td::Vec3& p2, const td::Color& c1, const td::Color& c2, bool use_depth);
typedef void(__fastcall* highlightShape)(TDRenderer* renderer, TDShape* shape, float opacity);
typedef void(__fastcall* outlineShape)(TDRenderer* renderer, TDShape* shape, td::Color* colour, float opacity);
typedef void(__fastcall* outlineBody)(TDRenderer* renderer, TDBody* body, float opacity);
typedef void(__fastcall* wrappedDoDamage)(TDScene* scene, td::Vec3* position, float damageA, float damageB, int unkn1, int* unkn2);
typedef void(__fastcall* setBodySomething)(uintptr_t body, __int8 a1, __int8 a2);
typedef void(__fastcall* handleBombObjs)(uintptr_t a1);
typedef void(__fastcall* setShapeParentBody)(TDShape* shape, byte unkn, TDBody* body);

//creation
typedef uintptr_t(__fastcall* tSpawnVox) (td::small_string* path, td::small_string* subpath, float scale);
typedef uintptr_t(__fastcall* spawnParticle)(DWORD* ptr, td::particleInfo* info, td::Vec3& position, td::Vec3& velocity, float lifetime);
typedef void(__fastcall* createExplosion)(uintptr_t scene, td::Vec3* pos, float radius);
typedef void(__fastcall* createLight)(TDRenderer* renderer, td::Vec3* a2, td::Vec3* a3, float a4, float a5, float a6, float a7);
typedef void(__fastcall* spawnFire)(uintptr_t scene, td::Vec3* pos);
typedef void(__fastcall* CreateTexture) (uintptr_t ptr);
typedef void(__fastcall* CreatePhysics) (uintptr_t ptr);
typedef void(__fastcall* createProjectile)(TDScene* scene, td::Vec3* pos, td::Vec3* dir, INT32 type, float unkn);
typedef void(__fastcall* deleteBody) (INT64 a1, INT64 a2, INT64 a3, INT64 a4, INT64 a5, INT64 a6, INT64 a7, INT64 a8);
typedef void(__fastcall* UpdateShapes) (uintptr_t ptr);
typedef void(__fastcall* B_Constructor) (uintptr_t ptr, uintptr_t parent);
typedef uintptr_t(__fastcall* S_Constructor) (uintptr_t ptr, uintptr_t parent);
typedef void(__fastcall* SetDynamic) (uintptr_t ptr, bool dynamic);
typedef void(__fastcall* SetObjectAttribute) (TDShape* shape, td::small_string* attribute, td::small_string* attribute2);
typedef void(__fastcall* SetObjectAttributeSingle) (TDShape* shape, td::small_string* attribute);
typedef void(__fastcall* ConstructJoint) (uintptr_t joint);
typedef void(__fastcall* AttachJoint) (void* a1, void* a2, void* a3, void* a4, void* a5);
typedef int(__fastcall* ReadSubobjectsFromVox) (td::small_string* path, int* out);
typedef void*(__fastcall* unknReadVoxData) (void* a1, void* a2);
//joints
typedef void(__fastcall* joinConstructor)(TDJoint* joint, Entity* parent);
typedef void(__fastcall* initBall)(TDJoint* joint, td::Vec3* transform, float size, float strengh, float springiness, bool collide);
typedef void(__fastcall* initHinge)(TDJoint* joint, td::Vec3* transform, float size, int64_t strengh, float springiness, bool collide, td::Vec2* limits);
typedef void(__fastcall* initPrismatic)(TDJoint* joint, td::Vec3* transform, float size, bool collide, td::Vec2* limits);
typedef void(__fastcall* initWire)(TDJoint* joint, td::Vec3* p1, td::Vec3* p2, float size, td::Vec4 color, float slack, float strengh, float maxStretch);
typedef void(__fastcall* attach)(TDJoint* joint, TDShape* shape1, TDShape* shape2, td::Vec3* pos1, td::Vec3* pos2);
typedef void(__fastcall* update)(TDJoint* joint);

//misc
typedef void(__stdcall* damageObject)(uintptr_t a1, uintptr_t a2, td::Vec3* a3, float a4, float a5, uintptr_t a6, uintptr_t a7, uintptr_t a8);
typedef void(__stdcall* createExplosionWrapped)(double unkn, td::Vec3* pos, float power);
typedef void(__fastcall* spawnParticleWrapped)(double a1, __int64 a2);
typedef void(__fastcall* tdMain)(TDGame* game, void* a2);
typedef BOOL(WINAPI* hk_SetCursorPos)(int, int);
typedef BOOL(__stdcall* twglSwapBuffers)(_In_ HDC hDc);
typedef void (*tPaint) (uintptr_t* Scene, td::Vec3* Position, float size, int darken, float dispersion); 
typedef void(__fastcall* cameraPositioning)(uintptr_t a1, float a2, uintptr_t a3, uintptr_t a4);
typedef void(__fastcall* modApiLinker)(__int64 a1);
typedef int(__fastcall* makeHole)(byte data1[32], byte data2[32], byte data3[32], byte data4[32]);
typedef char(__fastcall* idfk) (__int64 a1, __int64 a2, signed int* a3, signed int* a4, signed int* a5);
typedef void(__fastcall* damagePlayer) (TDPlayer* player, float damage);
typedef uintptr_t(__fastcall* TMalloc)(size_t);
typedef void(__fastcall* TFree)(uintptr_t mem);
typedef void(__fastcall* spreadFire)(__int64 a1, float v2);
typedef void(__fastcall* addContextItem)(char* a1, int a2, int a3, float* a4);

//td maths
typedef __int64(__fastcall* apiQuatEuler)(float* a1, float* a2);

//SEE TDFUNCS.CPP FOR SIGSCANNING
namespace glb {
    void setObjectAttribute(TDShape* shape, const char* a1, const char* a2);

    extern TDObjectList* TDOL;
    extern TDPlayer* player;
    extern TDGame* game;
    extern TDRenderer* renderer;
    extern TDScene* scene;
    extern tdMain oTDmain;
    extern wrappedDoDamage oWrappedDamage;
    extern setBodySomething oSetBody;
    extern ConstructJoint oConstructJoint;
    extern AttachJoint oAttachJoint;
    extern handleBombObjs oHandleBombObjs;
    extern setShapeParentBody oSetShapeParentBody;
    extern unknReadVoxData oIUnReadVox;

    extern highlightShape oHighlightShape;
    extern outlineShape oOutlineShape;
    extern outlineBody oOutlineBody;
    extern SetObjectAttribute oSOA;
    extern SetObjectAttributeSingle oSOAS;
    extern spreadFire oSpreadFire;
    extern environmentUpdate oEnvUpdate;
    extern createLight oCreateLight;
    extern deleteBody oTdDelBdy;
    extern cameraPositioning oCamPos;
    extern damageObject oDamageObject;
    extern modApiLinker oMAL;
    extern makeHole oMakeHole;
    extern damagePlayer oDamagePlayer;
    extern idfk killme;
    extern tPaint oPaint;
    extern tSpawnVox oSpawnVox;
    extern CreateTexture oCreateTexture;
    extern CreatePhysics oCreatePhysics;
    extern UpdateShapes oUpdateShapes;
    extern B_Constructor oB_Constructor;
    extern S_Constructor oS_Constructor;
    extern SetDynamic oSetDynamic;
    extern TMalloc oTMalloc;
    extern TFree oTFree;
    extern frameDrawLine oFDL;
    extern rayCast oRC;
    extern spawnFire oSpawnFire;
    extern createProjectile oPewpew;
    
    extern createExplosion TDcreateExplosion;
    extern spawnParticle TDspawnParticle;
    extern createExplosionWrapped TDcreateExplosionWrapped;
    extern spawnParticleWrapped TDspawnParticleWrapped;
    extern ReadSubobjectsFromVox TDreadSubobjects;

    extern uintptr_t moduleBase;
    extern twglSwapBuffers owglSwapBuffers;
    extern bool displayMenu;
    extern HWND gWnd;
    extern hk_SetCursorPos ocursor;
    extern HMODULE hMdl;

    extern apiQuatEuler oQuatEul;
    extern addContextItem oAddCItem;

    extern uintptr_t plankPatchFunction;
}
