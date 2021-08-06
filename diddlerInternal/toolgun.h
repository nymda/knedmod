#pragma once
namespace toolgun {

    enum class spawngunMethod : int {
        placed = 0,
        thrown = 1
    };

    enum class tgSettings : int {
        spawner = 0,
        weld = 1,
        rope = 2,
        minigun = 3,
        explosion = 4,
        flamethrower = 5,
        remover = 6,
        setAtttibute = 7,
        destroyer = 8,
        debugObject = 9,
        leafblower = 10,
        slicer = 11,
        camera = 12,
        destructionGrid = 13,
        testing = 14
    };

    struct targetDbgInfo {
        TDShape* tShape;
        TDBody* tBody;
    };

    struct fadeShapeOutline {
        TDShape* shape;
        int alpha = 255;
    };

    enum class leafblowerModes : int {
        blow = 0,
        succ = 1,
        del = 2
    };
    
    extern float ropeSlack;
    extern float ropeStrength;
    extern float ropeMaxStretch;
    extern td::Color ropeColor;
    extern float cameraFov;
    extern bool slicerHorizontal;
    extern bool takeSnapshot;
    extern int cameraResolution;
    extern targetDbgInfo dbgObject;
    extern char setAttributeFirst[128];
    extern char setAttributeSecond[128];
    extern bool playerIsHoldingToolgun;
    extern bool constSpawn;
    extern spawngunMethod method;
    extern float thrownObjectVelocityMultiplier;
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
    extern leafblowerModes LBMode;
    extern int leafBlowerRayCount;
    extern float leafBlowerFOV;
    extern bool showRayHitPos;
    extern float slicer_maxDist;
    extern int slicer_resolution;
    extern spawner::LoadedSpawnableObject currentSpawngunObject;
    extern tgSettings currentsetting;
    void handleToolgun();
}