#include "objectSpawner.h"
#include "drawCube.h"

namespace objectTesting {
	spawner::KMSpawnedObject dbgObject;
	td::Color white{ 1.f, 1.f, 1.f, 1.f };
	td::Color red{ 1.f, 0.f, 0.f, 1.f };
	td::Color green{ 0.f, 1.f, 0.f, 1.f };
	td::Color blue{ 0.f, 0.f, 1.f, 1.f };

	void spawnDebugObject() {
		spawner::objectSpawnerParams osp{};
		
		dbgObject = spawner::spawnObjectProxy("vox\\Default\\knedcube\\object.vox", osp);
	}

	void drawCircleAroundPoint(td::Vec3 vPoint, td::Vec3 vUp) {
		glm::vec3 gvPoint = glm::vec3(vPoint.x, vPoint.y, vPoint.z);
		glm::vec3 gvUp = glm::vec3(vUp.x, vUp.y, vUp.z);
		glm::quat qUp = glm::quat(gvUp);

		glm::vec3 dir1 = qUp * glm::vec3(1, 0, 0);
		glm::vec3 pos1 = { gvPoint.x + dir1.x, gvPoint.y + dir1.y, gvPoint.z + dir1.z };
		glb::oFDL(glb::renderer, vPoint, { pos1.x, pos1.y, pos1.z }, red, red, false);

		glm::vec3 dir2 = qUp * glm::vec3(-1, 0, 0);
		glm::vec3 pos2 = { gvPoint.x + dir2.x, gvPoint.y + dir2.y, gvPoint.z + dir2.z };
		glb::oFDL(glb::renderer, vPoint, { pos2.x, pos2.y, pos2.z }, red, red, false);
	}

	void drawTargetCircle() {
		raycaster::rayData rd = raycaster::castRayPlayer();
		drawCircleAroundPoint(rd.worldPos, rd.angle);
	}

	void updateDebugObject() {
		if (!dbgObject.body)
		{
			return;
		}


		drawCube(dbgObject.body->Position, 0.05f, white);

		glm::vec3 bodyPos = glm::vec3(dbgObject.body->Position.x, dbgObject.body->Position.y, dbgObject.body->Position.z);
		glm::quat bodyQuat = *(glm::quat*)&dbgObject.body->Rotation;

		glm::vec3 vx = bodyQuat * glm::vec3(1, 0, 0);
		glm::vec3 vy = bodyQuat * glm::vec3(0, 1, 0);
		glm::vec3 vz = bodyQuat * glm::vec3(0, 0, 1); //(UP)

		td::Vec3 oSize = { dbgObject.vox->sizeX, dbgObject.vox->sizeZ, dbgObject.vox->sizeZ };

		//draw x line
		td::Vec3 vx_p1 = dbgObject.body->Position;
		td::Vec3 vx_p2 = { dbgObject.body->Position.x + (vx.x * (oSize.x / 10.f)), dbgObject.body->Position.y + (vx.y * (oSize.y / 10.f)), dbgObject.body->Position.z + (vx.z * (oSize.z / 10.f)) };
		glb::oFDL(glb::renderer, vx_p1, vx_p2, red, red, false);

		//draw y line
		td::Vec3 vy_p1 = dbgObject.body->Position;
		td::Vec3 vy_p2 = { dbgObject.body->Position.x + (vy.x * (oSize.x / 10.f)), dbgObject.body->Position.y + (vy.y * (oSize.y / 10.f)), dbgObject.body->Position.z + (vy.z * (oSize.z / 10.f)) };
		glb::oFDL(glb::renderer, vy_p1, vy_p2, green, green, false);

		//draw z line
		td::Vec3 vz_p1 = dbgObject.body->Position;
		td::Vec3 vz_p2 = { dbgObject.body->Position.x + (vz.x * (oSize.x / 10.f)), dbgObject.body->Position.y + (vz.y * (oSize.y / 10.f)), dbgObject.body->Position.z + (vz.z * (oSize.z / 10.f)) };
		glb::oFDL(glb::renderer, vz_p1, vz_p2, blue, blue, false);

		//dbgObject.body->Rotation = glb::player->cameraQuat;

		//glm::vec3 translation = ((vz * 0.85f) + (vy * 0.15f) + (vx * 0.15f));
		//float spawnPosx = ((glb::player->cameraPosition.x) + (glb::player->cameraEuler().x * 2.25f));
		//float spawnPosy = ((glb::player->cameraPosition.y) + (glb::player->cameraEuler().y * 2.25f));
		//float spawnPosz = ((glb::player->cameraPosition.z) + (glb::player->cameraEuler().z * 2.25f));

		//dbgObject.body->Position = { spawnPosx - translation.x,  spawnPosy - translation.y, spawnPosz - translation.z };
		//dbgObject.body->Velocity = { 0.f, 0.f, 0.f };

		//dbgObject.body->countDown = 0x20;
		//dbgObject.body->isAwake = true;



		//drawCube({ translation.x, translation.y, translation.z }, 0.05f, white);
	}
}

