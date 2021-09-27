#include "tgtBase.h"
#include "tgtSpawner.h"
#include <iostream>
#include "Raycaster.h"
#include "snapPoints.h"

tgt_spawner::tgt_spawner() {
	tool = TOOL_SPAWNER;

    for (spawner::spawnerCatagory catig : spawner::spawnerObjectsDatabase) {
        for (spawner::LoadedSpawnableObject object : catig.objects) {
            if (object.objectName == "brick_metal") {
                currentSpawngunObject = object;
                break;
            }
        }
    }
}

DWORD tgt_spawner::exec() {
    raycaster::rayData rd = raycaster::castRayPlayer();

    td::Vec3 target = rd.worldPos;
    TDShape* tShape = rd.hitShape;

    if (rd.successful) {
        //snapPoints::drawSnapPoints(tShape);
    }



    if (!spawner::thrownMode) {
        spawner::drawSpawngunObjectOutline(currentSpawngunObject.voxObject, rd);
    }

    if (glb::player->isAttacking) {
        if (spawnOnce || constSpawn) {
            spawnOnce = false;
            if (spawner::freeMode) {
                spawner::freeObjectSpawnParams params = {};
                params.attributes = currentSpawngunObject.attributes;
                params.useUserRotation = true;
                params.nocull = true;
                spawner::placeFreeObject(currentSpawngunObject.voxPath, params);
            }
            else if (spawner::childMode) {
                spawner::childObjectSpawnParams params = {};
                params.attributes = currentSpawngunObject.attributes;
                params.useUserRotation = true;
                params.nocull = true;
                spawner::placeChildObject(currentSpawngunObject.voxPath, params);
            }
            else if (spawner::thrownMode) {
                spawner::thrownObjectSpawnParams params = {};
                params.attributes = currentSpawngunObject.attributes;
                params.power = throwPower;
                params.nocull = true;
                spawner::throwFreeObject(currentSpawngunObject.voxPath, params);
            }
        }
    }
    else {
        spawnOnce = true;
    }

	return 0x01;
}
