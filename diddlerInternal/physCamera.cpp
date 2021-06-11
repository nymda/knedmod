#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "drawCube.h"
#include "toolgun.h"
#include "camera.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "camera.h"
#include "dotProjector.h"

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

    int lastResolution = 64;
    bool flip = true;
    byte* frameBuffer;
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
                camera::constructFrameManual(pixelsColor, res, false);
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

        glm::quat cameraQuat = *(glm::quat*)(&camera.body->Rotation);
        glm::vec3 cameraUp = cameraQuat * glm::vec3(0, 0, 1);;


        camera::drawCameraWindow();

        if (camera::interlaceMode) {
            if (toolgun::cameraResolution != lastResolution || !frameBuffer) {
                free(frameBuffer);
                frameBuffer = new byte[(toolgun::cameraResolution * toolgun::cameraResolution) * 4];
            }
            lastResolution = toolgun::cameraResolution;
            if (camera::transparency) {
                rcf.m_RejectTransparent = true;
            }
            else {
                rcf.m_RejectTransparent = false;
            }
            flip = !flip;
            camera::interlacedImage(frameBuffer, toolgun::cameraResolution, flip, fov, 1.f, (glm::quat*)&camera.body->Rotation, centerpoint, { -1, 0, 0 }, { cameraUp.x, cameraUp.y, cameraUp.z }, &rcf);
            camera::constructFrameManual(frameBuffer, toolgun::cameraResolution, false);
        }
        else {
            dotProjector::pixelResponse* response = dotProjector::projectDotMatrix(toolgun::cameraResolution, fov, 1.f, true, (glm::quat*)&camera.body->Rotation, centerpoint, { 0, 0, -1 }, { 0, 1, 0 }, &rcf);
            if (camera::colourMode) {
                camera::constructColourFrame(response, toolgun::cameraResolution, false);
            }
            else {
                camera::constructDistanceFrame(response, toolgun::cameraResolution, response->minDist, response->maxDist);
            }
        }
	}
}
