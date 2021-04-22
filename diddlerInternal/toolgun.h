#pragma once
namespace toolgun {
    enum class tgSettings : int {
        spawner = 0,
        minigun = 1,
        explosion = 2,
        flamethrower = 3,
        remover = 4,
        setAtttibute = 5,
        testing = 6,
    };

    struct fadeShapeOutline {
        TDShape* shape;
        int alpha = 255;
    };

    extern char setAttributeFirst[128];
    extern char setAttributeSecond[128];

    extern float spreadVal;
    extern int bulletsPerFrame;
    extern float EXspreadVal;
    extern int EXbulletsPerFrame;
    extern float flRadius;
    extern int chance;
    extern int minigunBulletType;
    extern int particalID;
    extern float power;

    extern spawner::LoadedSpawnableObject currentSpawngunObject;
    extern tgSettings currentsetting;
    void handleToolgun();
}