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

		//draw x line
		td::Vec3 vx_p1 = dbgObject.body->Position;
		td::Vec3 vx_p2 = { dbgObject.body->Position.x + vx.x, dbgObject.body->Position.y + vx.y, dbgObject.body->Position.z + vx.z };
		glb::oFDL(glb::renderer, vx_p1, vx_p2, red, red, false);

		//draw y line
		td::Vec3 vy_p1 = dbgObject.body->Position;
		td::Vec3 vy_p2 = { dbgObject.body->Position.x + vy.x, dbgObject.body->Position.y + vy.y, dbgObject.body->Position.z + vy.z };
		glb::oFDL(glb::renderer, vy_p1, vy_p2, green, green, false);

		//draw z line
		td::Vec3 vz_p1 = dbgObject.body->Position;
		td::Vec3 vz_p2 = { dbgObject.body->Position.x + vz.x, dbgObject.body->Position.y + vz.y, dbgObject.body->Position.z + vz.z };
		glb::oFDL(glb::renderer, vz_p1, vz_p2, blue, blue, false);

		glm::vec3 center = bodyPos + ((vx / 2.f) +  (vy / 2.f));
		drawCube({ center.x, center.y, center.z }, 0.05f, white);

		dbgObject.body->Velocity = { dbgObject.body->Velocity.x + (vz.x * 5.f), dbgObject.body->Velocity.y + (vz.y * 5.f), dbgObject.body->Velocity.z + (vz.z * 5.f) };
	}
}

