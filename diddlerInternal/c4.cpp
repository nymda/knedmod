#include "global.h"
#include "Raycaster.h"
#include "objectSpawner.h"

namespace c4 {
    std::vector<spawner::KMSpawnedObject> explosiveObjects = {};
    bool isDetonating = false;

    bool runOnce = true;
    void runC4() {
        const char* c4name = "cfour";
        if (memcmp(glb::player->heldItemName, c4name, 5) == 0) {
            td::Vec4 target = castRayPlayer();

            if (glb::player->isAttacking == true && !isDetonating) {
                if (runOnce) {
                    //runOnce = false;

                    td::Vec3 cameraDirection = glb::player->cameraEuler();
                    spawner::objectSpawnerParams osp;
                    osp.spawnType = spawner::objectSpawnType::thrown;
                    osp.startVelocity = { cameraDirection.x * 20, cameraDirection.y * 20, cameraDirection.z * 20 };
                    osp.rotateFacePlayer = true;
                    explosiveObjects.push_back(spawner::spawnObjectProxy("vox\\c4\\object.vox", osp));
                }
            }
            else {
                runOnce = true;
            }
        }

        const char* c4detName = "cfDet";
        if (memcmp(glb::player->heldItemName, c4detName, 5) == 0) {
            td::Vec4 target = castRayPlayer();

            if (glb::player->isAttacking == true) {
                if (runOnce) {
                    runOnce = false;
                    //isDetonating = true;
                    for (spawner::KMSpawnedObject KMSPO : explosiveObjects) {
                        td::Vec3 objectMin = KMSPO.shape->min;
                        td::Vec3 objectMax = KMSPO.shape->max;
                        td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                        glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, 2.0);
                    }
                    explosiveObjects.clear();
                }
            }
            else {
                runOnce = true;
            }
        }

       /* if (isDetonating) {
            if (explosiveObjects.size() > 0) {
                spawner::KMSpawnedObject current = explosiveObjects.front();
                td::Vec3 objectMin = current.shape->min;
                td::Vec3 objectMax = current.shape->max;
                td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, 3.0);
                explosiveObjects.erase(explosiveObjects.begin());
            }
            else {
                isDetonating = false;
            }
        }*/
    }
}

