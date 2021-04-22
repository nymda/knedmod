#include "objectSpawner.h"
#include "lantern.h"

namespace lantern {
    spawner::KMSpawnedObject lantern;
    float a1 = 1.f;
    float a2 = 1.f;
    float a3 = 1.f;
    float a4 = 1.f;
    float a5 = 1.f;
    float lightR = 1.f;
    float lightG = 1.f;
    float lightB = 1.f;

    void spawnLantern() {
        if (lantern.body) {
            lantern.body->Destroy(lantern.body, true);
            lantern.shape->Destroy(lantern.shape, true);
        }

        spawner::objectSpawnerParams params = {};
        params.nocull = true;
        params.unbreakable = true;
        params.pushSpawnList = false;
        lantern = spawner::spawnObjectProxy("vox\\default\\lantern\\object.vox", params);
    }

    void updateLantern() {
        if (glb::game->State == gameState::menu) {
            lantern = {};
        }

        if (lantern.body) {
            td::Vec3 objectMin = lantern.shape->posMin;
            td::Vec3 objectMax = lantern.shape->posMax;
            td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
            td::Vec3 col = { lightR, lightG, lightB };
            //glb::oCreateLight(glb::renderer, &centerpoint, &col, a1, a2, a3, a4, 0.1f);
            glb::oCreateLight(glb::renderer, &centerpoint, &col, 0.1f, 0.1f, 1.5f, 1.5f);
        }
    }
}

