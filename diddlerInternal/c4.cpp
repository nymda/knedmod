#include "global.h"
#include "Raycaster.h"
#include "objectSpawner.h"
#include "TDObjects.h"
#include "Mods.h"
#include "c4.h"

namespace c4 {

    static const char* bombSizeStr[] = {
        "vox\\Default\\c4_small\\object.vox",
        "vox\\Default\\c4_medium\\object.vox",
        "vox\\Default\\c4_large\\object.vox",
        "vox\\Default\\c4_huge\\object.vox"
    };

    struct spawnedC4 {
        spawner::KMSpawnedObject object;
        int explosionSize;
    };

    struct thrownFirecracker {
        int frameCountdown = 120;
        spawner::KMSpawnedObject object;
    };

    int selectedBombSizeInt = 1;
    std::vector<spawnedC4> explosiveObjects = {};
    bool isDetonating = false;

    void cleanup() {
        explosiveObjects.clear();
    }

    int frameCount = 0;
    bool runOnce = true;
    bool runOnceImp = true;
    spawner::KMSpawnedObject impNade;
    std::vector<thrownFirecracker> thrownCrackers = {};

    void runC4() {
        if (frameCount == 0) {
            runOnce = true;
        }
        else {
            frameCount--;
        }

        if (glb::game) {
            if (glb::game->State == gameState::menu) {
                if (explosiveObjects.size() > 0) {
                    std::cout << "Cleaning up c4 objects" << std::endl;
                    cleanup();
                }
            }
        }

        const char* c4name = "cfour";
        if (memcmp(glb::player->heldItemName, c4name, 5) == 0) {
            if (glb::player->isAttacking == true && !isDetonating) {
                if (runOnce) {
                    runOnce = false;
                    frameCount = 30;

                    raycaster::rayData rd = raycaster::castRayPlayer();
                    td::Vec3 target = rd.worldPos;

                    td::Vec3 cameraDirection = glb::player->cameraEuler();
                    spawner::objectSpawnerParams osp;
                    osp.spawnType = spawner::objectSpawnType::thrown;
                    osp.startVelocity = { cameraDirection.x * 20, cameraDirection.y * 20, cameraDirection.z * 20 };
                    osp.rotateFacePlayer = true;
                    osp.unbreakable = true;
                    osp.nocull = true;
                    osp.pushSpawnList = false;
                    //osp.customRotation = rd.angle;

                    const char* currentPath = bombSizeStr[selectedBombSizeInt-1];
                    spawner::KMSpawnedObject object = spawner::spawnObjectProxy(currentPath, osp);

                    if (selectedBombSizeInt == 4) {
                        explosiveObjects.push_back({ object,  10 });
                    }
                    else {
                        explosiveObjects.push_back({ object,  selectedBombSizeInt });
                    }
                }
            }
            else {
                frameCount = 0;
                runOnce = true;
            }
        }

        const char* c4detName = "cfDet";
        if (memcmp(glb::player->heldItemName, c4detName, 5) == 0) {
            if (glb::player->isAttacking == true) {
                if (runOnce) {
                    raycaster::rayData rd = raycaster::castRayPlayer();
                    td::Vec3 target = rd.worldPos;

                    runOnce = false;
                    isDetonating = true;
                    if (mods::c4_global_detonation) {
                        for (spawnedC4 curC4 : explosiveObjects) {
                            td::Vec3 objectMin = curC4.object.shape->posMin;
                            td::Vec3 objectMax = curC4.object.shape->posMax;
                            td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                            glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, curC4.explosionSize);
                            curC4.object.shape->attributes = 0x00;
                            curC4.object.shape->Destroy(curC4.object.shape, true);
                            curC4.object.body->Destroy(curC4.object.body, true);
                        }
                        explosiveObjects.clear();
                    }
                }
            }
            else {
                runOnce = true;
            }
        }

        const char* impname = "cracker";
        if (memcmp(glb::player->heldItemName, impname, 7) == 0) {
            if (glb::player->isAttacking == true) {
                if (runOnceImp) {
                    runOnceImp = false;

                    raycaster::rayData rd = raycaster::castRayPlayer();
                    td::Vec3 target = rd.worldPos;

                    td::Vec3 cameraDirection = glb::player->cameraEuler();
                    spawner::objectSpawnerParams osp;
                    osp.spawnType = spawner::objectSpawnType::thrown;
                    osp.startVelocity = { cameraDirection.x * 20, cameraDirection.y * 20, cameraDirection.z * 20 };
                    osp.rotateFacePlayer = true;
                    osp.unbreakable = false;
                    osp.nocull = true;
                    osp.pushSpawnList = false;
                    
                    const char* currentPath = "vox\\Default\\Cracker\\object.vox";
                    impNade = spawner::spawnObjectProxy(currentPath, osp);

                    thrownFirecracker tfc = { 120, impNade };
                    thrownCrackers.push_back(tfc);
                }
            }
            else {
                frameCount = 0;
                runOnceImp = true;
            }
        }

        bool hasActiveCracker = false;
        for (thrownFirecracker& tfc : thrownCrackers)
        {
            if (tfc.frameCountdown >= 0) {
                uintptr_t special = *(uintptr_t*)((uintptr_t)glb::scene + 0x68);
                td::Vec3 objectMin = tfc.object.shape->posMin;
                td::Vec3 objectMax = tfc.object.shape->posMax;
                td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                td::Vec3 vel = { 0, 0, 0 };

                td::particleInfo tdpi = { 0.f, 0.f, 0.f, 0.7f, 0.7f, 0.7f, 1.f, 0.7f, 0.7f, 0.7f, 1.f, 0.f, 0.f, 0.f, 0.2f, 0.f, 0.f, 0.f, 0.f, 0.15f, 0.25f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
                glb::TDspawnParticle((DWORD*)special, &tdpi, centerpoint, vel, 1.f);

                hasActiveCracker = true;
                tfc.frameCountdown--;
            }
            if(tfc.frameCountdown == 0){
                td::Vec3 objectMin = tfc.object.shape->posMin;
                td::Vec3 objectMax = tfc.object.shape->posMax;
                td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, 0.f);
                tfc.object.shape->attributes = 0x00;
                tfc.object.shape->Destroy(tfc.object.shape, true);
                tfc.object.body->Destroy(tfc.object.body, true);
            }
        }
        if (!hasActiveCracker) {
            thrownCrackers.clear();
        }

        //*(byte*)((uintptr_t)(current.shape) + 9) = 0x00;

        if (isDetonating) {
            if (!mods::c4_global_detonation) {
                if (explosiveObjects.size() > 0) {
                    spawnedC4 curC4 = explosiveObjects.front();
                    td::Vec3 objectMin = curC4.object.shape->posMin;
                    td::Vec3 objectMax = curC4.object.shape->posMax;
                    td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                    glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, curC4.explosionSize);
                    curC4.object.shape->attributes = 0x00;
                    curC4.object.shape->Destroy(curC4.object.shape, true);
                    curC4.object.body->Destroy(curC4.object.body, true);
                    explosiveObjects.erase(explosiveObjects.begin());
                }
                else {
                    isDetonating = false;
                }
            }
            else {
                isDetonating = false;
            }
        }
    }
}

