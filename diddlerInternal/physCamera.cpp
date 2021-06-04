#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "drawCube.h"
#include "toolgun.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"

namespace physCamera {
	spawner::KMSpawnedObject camera = {};
	RaycastFilter rcf = {};
	td::Color blue{ 0.f, 0.f, 1.f, 1.f };
	td::Color red{ 1.f, 0.f, 0.f, 1.f };
	const float pi = 3.1415926535f;

	void spawnCameraObject() {
		spawner::objectSpawnerParams osp = {};
		osp.unbreakable = true;
		osp.nocull = true;
		camera = spawner::spawnObjectProxy("vox/Default/Camera/object.vox", osp);

		rcf.m_IgnoredBodies.push_back(camera.body);
		rcf.m_IgnoredShapes.push_back(camera.shape);
	}

	bool genFilter = true;
	void updateCamera() {
		if (!camera.body) {
			return;
		}

		td::Vec3 objectMin = camera.shape->posMin;
		td::Vec3 objectMax = camera.shape->posMax;
		td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };

		glm::quat pCamera = *(glm::quat*)(&camera.body->Rotation);
		glm::vec3 camPYR = glm::eulerAngles(pCamera);
		glm::quat modifiedCamQuat1 = glm::quat({ camPYR.x - (pi / 8), camPYR.y - (pi / 8), camPYR.z });
		glm::quat modifiedCamQuat2 = glm::quat({ camPYR.x + (pi / 8), camPYR.y + (pi / 8), camPYR.z });

		glm::vec3 castDir1 = modifiedCamQuat1 * glm::vec3(-1, 0, 0);
		glm::vec3 castDir2 = modifiedCamQuat2 * glm::vec3(-1, 0, 0);

		raycaster::rayData rd = raycaster::castRayManual(centerpoint, { castDir1.x, castDir1.y, castDir1.z }, &rcf);
		glb::oFDL(glb::renderer, centerpoint, rd.worldPos, blue, blue, false);

		rd = raycaster::castRayManual(centerpoint, { castDir2.x, castDir2.y, castDir2.z }, &rcf);
		glb::oFDL(glb::renderer, centerpoint, rd.worldPos, blue, blue, false);



	}
}
