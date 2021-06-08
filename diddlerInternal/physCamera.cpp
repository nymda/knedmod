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
#include "camera.h"

namespace physCamera {

	spawner::KMSpawnedObject camera = {};
	RaycastFilter rcf = {};
	td::Color blue{ 0.f, 0.f, 1.f, 1.f };
	td::Color red{ 1.f, 0.f, 0.f, 1.f };
	const float pi = 3.1415926535f;

	void spawnCameraObject() {
		spawner::objectSpawnerParams osp = {};
		//osp.unbreakable = true;
		osp.nocull = true;
		camera = spawner::spawnObjectProxy("vox/Default/Camera/object.vox", osp);

		rcf.m_IgnoredBodies.push_back(camera.body);
		rcf.m_IgnoredShapes.push_back(camera.shape);
	}

	bool genFilter = true;
    float* pixels = nullptr;
    float minDist = 1000.f;
    float maxDist = 0.f;
    int deadCameraframes = 0;

    void destroyCamera() {
        camera.body->Destroy(camera.body, true);
        camera.shape->Destroy(camera.shape, true);
        camera = {};
    }

    float randFloat(float min, float max) {
        return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    }

    byte* pixelsColor = nullptr;
	void updateCamera() {
        int res = toolgun::cameraResolution;
        float fov = toolgun::cameraFov;
        int pixelOffset = 0;

		if (!camera.body || !camera.shape) {

			return;
		}
  
        if (camera.shape->isBroken) {
            if (deadCameraframes > 0) {
                free(pixelsColor);
                pixelsColor = new byte[(res * res) * 4];

                for (int y = res; y > 0; y--) {
                    for (int x = 0; x < res; x++) {
                        int shade = rand() % 255;
                        pixelsColor[pixelOffset] = (byte)(shade);
                        pixelsColor[pixelOffset + 1] = (byte)(shade);
                        pixelsColor[pixelOffset + 2] = (byte)(shade);
                        pixelsColor[pixelOffset + 3] = (byte)(255);
                        pixelOffset += 4;
                    }
                }
                camera::updateCameraFrameColor(pixelsColor, res, 0, 0, false);
                camera::drawCameraWindow();
                deadCameraframes--;
            }
            else {
                destroyCamera();
            }

            return;
        }

		td::Vec3 objectMin = camera.shape->posMin;
		td::Vec3 objectMax = camera.shape->posMax;
		td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
        deadCameraframes = 30;

        minDist = 1000.f;
        maxDist = 0.f;

        free(pixelsColor);
        pixelsColor = new byte[(res * res) * 4];

        camera::drawCameraWindow();

        td::Color red{ 1.f, 0.f, 0.f, 1.f };
        td::Color green{ 0.f, 1.f, 0.f, 1.f };
        td::Color blue{ 0.f, 0.f, 1.f, 1.f };
        td::Color white{ 1.f, 1.f, 1.f, 1.f };

        RaycastFilter filter{ 0 };
        filter.m_Mask = -1;
        filter.m_RejectTransparent = true;
        filter.m_IgnoredBodies.push_back(camera.body);

        glm::quat camera_rotation_bl = *(glm::quat*)(&camera.body->Rotation);
        glm::vec3 raycast_dir_bl = camera_rotation_bl * glm::vec3(-1, 0, 0);
        glm::vec3 raycasterUp = camera_rotation_bl * glm::vec3(0, 0, 1);
        td::VoxelsPaletteInfo palOut = {};

        raycaster::rayData rd = raycaster::castRayManual(centerpoint, { raycast_dir_bl.x, raycast_dir_bl.y, raycast_dir_bl.z }, &filter, &palOut);

        glm::vec3 glCameraPos = glm::vec3(centerpoint.x, centerpoint.y, centerpoint.z);
        glm::vec3 glTarget = glm::vec3(rd.worldPos.x, rd.worldPos.y, rd.worldPos.z);
        glm::mat4x4 vmatrix = glm::lookAt(glCameraPos, glTarget, raycasterUp);
        glm::mat4x4 pmatrix = glm::perspective(50.f, 1.f, 1.f, 150.f);

        glm::mat4 invProjMat = glm::inverse(pmatrix);
        glm::mat4 invViewMat = glm::inverse(vmatrix);

        for (int y = res; y > 0; y--) {
            for (int x = 0; x < res; x++) {
                float pxSize = (fov / res);
                float comX = (fov / 2.f) - (x * pxSize) + randFloat(-(pxSize / 3.f), (pxSize / 3.f));
                float comY = (fov / 2.f) - (y * pxSize) + randFloat(-(pxSize / 3.f), (pxSize / 3.f));

                glm::vec2 ray_nds = glm::vec2(comX, comY);
                glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
                glm::vec4 eyeCoords = invProjMat * ray_clip;
                eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
                glm::vec4 rayWorld = invViewMat * eyeCoords;
                glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
                td::VoxelsPaletteInfo palOut = {};
                rd = raycaster::castRayManual(centerpoint, { rayDirection.x, rayDirection.y, rayDirection.z }, &filter, &palOut);

                float thisDist = rd.distance;
                if (thisDist >= 1000.f) {
                    pixelsColor[pixelOffset] = (byte)(0);
                    pixelsColor[pixelOffset + 1] = (byte)(77);
                    pixelsColor[pixelOffset + 2] = (byte)(77);
                    pixelsColor[pixelOffset + 3] = (byte)(255);
                }
                else {
                    pixelsColor[pixelOffset] = (byte)(palOut.m_Color.m_R * 255);
                    pixelsColor[pixelOffset + 1] = (byte)(palOut.m_Color.m_G * 255);
                    pixelsColor[pixelOffset + 2] = (byte)(palOut.m_Color.m_B * 255);
                    pixelsColor[pixelOffset + 3] = (byte)(palOut.m_Color.m_A * 255);
                }
                pixelOffset += 4;
            }
        }
        camera::updateCameraFrameColor(pixelsColor, res, minDist, maxDist, toolgun::takeSnapshot);
	}
}
