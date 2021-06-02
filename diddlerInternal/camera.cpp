#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "drawCube.h"

namespace camera {

	int resolution = 32;
	td::Color red{ 1.f, 0.f, 0.f, 1.f };
	td::Color green{ 0.f, 1.f, 0.f, 1.f };
	td::Color blue{ 0.f, 0.f, 1.f, 1.f };
	td::Color white{ 1.f, 1.f, 1.f, 1.f };

	void showBounds() {
		RaycastFilter filter{ 0 };
		filter.m_Mask = -1;

		float pitchMin = glb::player->camPitch - 0.10f;
		float pitchMax = glb::player->camPitch + 0.10f;
		float yawMin = glb::player->camYaw - 0.10f;
		float yawMax = glb::player->camYaw + 0.10f;

		glm::quat camera(glm::vec3(glb::player->camPitch, glb::player->camYaw, 0));
		glm::vec3 forward = camera * glm::vec3(0, 0, -1);
		glm::quat horizontal(glm::vec3(0.25, 0, 0));
		glm::quat vertical(glm::vec3(0, 0.25, 0));

		glm::vec3 raycast_dir_tl = -horizontal * -vertical * forward;
		glm::vec3 raycast_dir_bl = -horizontal * vertical * forward;
		glm::vec3 raycast_dir_tr = horizontal * -vertical * forward;
		glm::vec3 raycast_dir_br = horizontal * vertical * forward;

		//glm::quat camera_rotation_tl = glm::quat(glm::vec3(pitchMax, yawMax, 0));
		//glm::vec3 raycast_dir_tl = camera_rotation_tl * glm::vec3(0, 0, -1);

		//glm::quat camera_rotation_tr = glm::quat(glm::vec3(pitchMax, yawMin, 0));
		//glm::vec3 raycast_dir_tr = camera_rotation_tr * glm::vec3(0, 0, -1);

		//glm::quat camera_rotation_bl = glm::quat(glm::vec3(pitchMin, yawMin, 0));
		//glm::vec3 raycast_dir_bl = camera_rotation_bl * glm::vec3(0, 0, -1);

		//glm::quat camera_rotation_br = glm::quat(glm::vec3(pitchMin, yawMax, 0));
		//glm::vec3 raycast_dir_br = camera_rotation_br * glm::vec3(0, 0, -1);


		raycaster::rayData rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_tl.x, raycast_dir_tl.y, raycast_dir_tl.z }, &filter);
		td::Vec3 target = rd.worldPos;
		drawCube({ target.x, target.y, target.z }, 0.05, red);

		rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_tr.x, raycast_dir_tr.y, raycast_dir_tr.z }, &filter);
		target = rd.worldPos;
		drawCube({ target.x, target.y, target.z }, 0.05, white);

		rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_bl.x, raycast_dir_bl.y, raycast_dir_bl.z }, &filter);
		target = rd.worldPos;
		drawCube({ target.x, target.y, target.z }, 0.05, green);

		rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_br.x, raycast_dir_br.y, raycast_dir_br.z }, &filter);
		target = rd.worldPos;
		drawCube({ target.x, target.y, target.z }, 0.05, blue);

	}

	void takeSnapshot() {
		float pitchMin = glb::player->camPitch - 0.5;
		float pitchMax = glb::player->camPitch + 0.5;
		float yawMin = glb::player->camYaw - 0.5;
		float yawMax = glb::player->camYaw + 0.5;
		float step = 1.0 / resolution;

		float currentPitch = pitchMin;
		float currentYaw = yawMin;

		for (int x = 0; x < resolution; x++) {
			currentPitch += step;
			for (int y = 0; y < resolution; y++) {
				currentYaw += step;
				glm::quat camera_rotation = glm::quat(glm::vec3(currentPitch, currentYaw, 0));
				glm::vec3 raycast_dir = camera_rotation * glm::vec3(0, 0, -1);

				RaycastFilter filter{ 0 };
				filter.m_Mask = -1;
				raycaster::rayData rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir.x, raycast_dir.y, raycast_dir.z }, &filter);
				td::Vec3 target = rd.worldPos;
				drawCube({ target.x, target.y, target.z }, 0.05, white);
			}
		}
	}
}
