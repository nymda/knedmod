#pragma once
namespace toolgun {
    enum class tgSettings : int {
        spawner = 0,
        minigun = 1,
        explosion = 2,
        flamethrower = 3,
        remover = 4,
        setAtttibute = 5,
        destroyer = 6,
        debugObject = 7,
        testing = 8,
    };

    struct targetDbgInfo {
        TDShape* tShape;
        TDBody* tBody;
    };

    struct fadeShapeOutline {
        TDShape* shape;
        int alpha = 255;
    };

    extern targetDbgInfo dbgObject;
    extern char setAttributeFirst[128];
    extern char setAttributeSecond[128];
    extern bool playerIsHoldingToolgun;

    extern float spreadVal;
    extern int bulletsPerFrame;
    extern float EXspreadVal;
    extern int EXbulletsPerFrame;
    extern float flRadius;
    extern int chance;
    extern int minigunBulletType;
    extern int particalID;
    extern float power;
    extern float maxRange;
    extern float holeSize;
    extern bool fireConst;

    extern spawner::LoadedSpawnableObject currentSpawngunObject;
    extern tgSettings currentsetting;
    void handleToolgun();
}