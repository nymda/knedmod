#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"

namespace camera {

	spawner::KMSpawnedObject camera;
	td::Color white{ 1.f, 1.f, 1.f, 1.f };

	void spawnCamera() {
		spawner::objectSpawnerParams params = {};
		params.unbreakable = true;
		params.nocull = true;
		camera = spawner::spawnObjectProxy("vox\\camera\\object.vox", params);
	}

	void runCamera() {
		if (camera.body != 0) {
			td::Vec3 objectMin = camera.shape->posMin;
			td::Vec3 objectMax = camera.shape->posMax;
			td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
			//td::Vec3 centerpoint = camera.body->Position;

			RaycastFilter rcf{ 0 };
			raycaster::rayData rayDat = raycaster::castRayManual(centerpoint, quat2euler(camera.body->Rotation, 1, true), rcf);
			glb::oFDL(glb::renderer, centerpoint, rayDat.worldPos, white, white, false);
		}
	}
}
