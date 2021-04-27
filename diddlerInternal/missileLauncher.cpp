#include <vector>
#include "Global.h"
#include "Raycaster.h"
#include "objectSpawner.h"
#include "TDObjects.h"
#include "Mods.h"
#include "maths.h"

namespace missile {
    struct missileObject {
        spawner::KMSpawnedObject obj;
        int age = 0;
        bool destroyed = false;
    };

	std::vector<spawner::KMSpawnedObject> launchedMissiles = {};
    missileObject missile;
    bool runOnce = false;
    td::Color white{ 1.f, 1.f, 1.f, 1.f };

	void launchMissile() {
        spawner::objectSpawnerParams osp;
        td::Vec3 cameraDirection = glb::player->cameraEuler();
        osp.spawnType = spawner::objectSpawnType::front;
        osp.customRotation = glb::player->cameraQuat;
        osp.rotateFacePlayer = true;
        osp.unbreakable = true;
        osp.nocull = true;
        osp.pushSpawnList = false;
        osp.startVelocity = { cameraDirection.x * 100, cameraDirection.y * 100, cameraDirection.z * 100 };

        const char* currentPath = "vox\\Default\\missile\\object.vox";
        missile = { spawner::spawnObjectProxy(currentPath, osp), 0 };
	}

    void runMissile() {

        if (glb::game->State == gameState::menu) {
            missile = {};
        }
          
        const char* launcherName = "launcher";
        if (memcmp(glb::player->heldItemName, launcherName, 8) == 0) {
            if (glb::player->isAttacking == true) {
                if (runOnce) {
                    launchMissile();
                    runOnce = false;
                }
            }
            else {
                runOnce = true;
            }
        }

        if (missile.obj.body) {        
            if (!missile.destroyed) {
                uintptr_t special = *(uintptr_t*)((uintptr_t)glb::scene + 0x68);
                missile.age++;

                td::Vec3 rotationEul = quat2euler(missile.obj.body->Rotation, 0, true);

                float totalSpeed = missile.obj.body->Velocity.x + missile.obj.body->Velocity.y + missile.obj.body->Velocity.z;

                if (totalSpeed < 100.f && totalSpeed > -100.f) {
                    missile.obj.body->Velocity.x += (rotationEul.x * 5);
                    missile.obj.body->Velocity.y += (rotationEul.y * 5);
                    missile.obj.body->Velocity.z += (rotationEul.z * 5);
                }

                std::cout << std::to_string(totalSpeed) << std::endl;

                RaycastFilter rcf = {};
                rcf.m_IgnoredBodies.push_back(missile.obj.body);
                rcf.m_IgnoredShapes.push_back(missile.obj.shape);

                td::Vec3 objectMin = missile.obj.shape->posMin;
                td::Vec3 objectMax = missile.obj.shape->posMax;
                td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };

                raycaster::rayData rayDat = raycaster::castRayManual(centerpoint, rotationEul, &rcf);
                //glb::oFDL(glb::renderer, centerpoint, rayDat.worldPos, white, white, false);

                td::Vec3 vel = { 0.f, 0.f, 0.f };
                td::particleInfo tdpi = { 0.f, 0.f, 0.f, 0.7f, 0.7f, 0.7f, 1.f, 0.7f, 0.7f, 0.7f, 1.f, 0.f, 0.f, 0.f, 0.2f, 0.f, 0.f, 0.f, 0.f, 0.15f, 0.25f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
                glb::TDspawnParticle((DWORD*)special, &tdpi, centerpoint, vel, 0.5f);

                if ((rayDat.distance < 2.f && missile.age > 10) || (missile.age > 300)) {
                    glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, 3.f);
                    missile.obj.shape->Destroy(missile.obj.shape, true);
                    missile.obj.body->Destroy(missile.obj.body, true);
                    missile.destroyed = true;
                }
            }
        }
    }
}