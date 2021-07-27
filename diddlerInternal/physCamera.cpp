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
#include <glm/gtx/quaternion.hpp>

namespace physCamera {

	spawner::spawnedObject camera = {};
	RaycastFilter rcf = {};
	td::Color blue{ 0.f, 0.f, 1.f, 1.f };
	td::Color red{ 1.f, 0.f, 0.f, 1.f };
	const float pi = 3.1415926535f;

    bool useExistingParentBody = true;

	void spawnCameraObject() {
		spawner::objectSpawnerParams osp = {};
		//osp.unbreakable = true;
		osp.nocull = true;
        if (useExistingParentBody) {
            camera = spawner::placeChildObject("vox/Default/camera/object.vox");
        }
        else {
            camera = spawner::placeFreeObject("vox/Default/camera/object.vox");
        }


		//rcf.m_IgnoredBodies.push_back(camera.body);
		rcf.m_IgnoredShapes.push_back(camera.shapes[0]);
	}

	bool genFilter = true;
    float* pixels = nullptr;
    float minDist = 1000.f;
    float maxDist = 0.f;
    int deadCameraframes = 0;

    void destroyCamera() {
        //camera.body->Destroy(camera.body, true);
        //camera.shape->Destroy(camera.shape, true);
        camera = {};
    }

    float randFloat(float min, float max) {
        return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    }

    int lastResolution = 64;
    bool flip = true;
    byte* frameBuffer;
    byte* pixelsColor = nullptr;
    std::chrono::steady_clock::time_point FRAMESTART;
    std::chrono::steady_clock::time_point FRAMEEND;
    std::chrono::high_resolution_clock execTimer;
    float fps = 0;

	void updateCamera() {

        FRAMESTART = execTimer.now();

        int res = toolgun::cameraResolution;
        float fov = toolgun::cameraFov;
        int pixelOffset = 0;

        if (camera.body == 0x00) {
            return;
        }

        TDShape* cameraShape = camera.shapes[0];
        TDBody* cameraBody = camera.body;

		if (!cameraBody || !cameraShape) {

			return;
		}
  
        if (cameraShape->isBroken) {
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
                camera::drawCameraWindow(fps);
                deadCameraframes--;
            }
            else {
                destroyCamera();
            }

            return;
        }

		td::Vec3 objectMin = cameraShape->posMin;
		td::Vec3 objectMax = cameraShape->posMax;
		//td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };

        glm::vec3 bodyPos = glm::vec3(camera.body->Position.x, camera.body->Position.y, camera.body->Position.z);
        glm::quat bodyQuat = *(glm::quat*)&cameraBody->Rotation;

        glm::vec3 cameraPosOffset = *(glm::vec3*)&cameraShape->pOffset;
        glm::quat cameraRotOffset = *(glm::quat*)&cameraShape->rOffset;
       
        //bodyQuat = cameraRotOffset * bodyQuat;

        glm::vec3 vx = bodyQuat * glm::vec3(1, 0, 0);
        glm::vec3 vy = bodyQuat * glm::vec3(0, 1, 0);
        glm::vec3 vz = bodyQuat * glm::vec3(0, 0, 1); //(UP)

        glm::vec3 offsetPos = bodyPos + ((vz * cameraPosOffset.z) + (vy * cameraPosOffset.y) + (vx * cameraPosOffset.x)); //this is the position of the camera IN RELATION TO THE WORLD

        //recreate the cameras rotation in relation to the world and recreate the 3 vectors
        bodyQuat = bodyQuat * cameraRotOffset;
        vx = bodyQuat * glm::vec3(1, 0, 0);
        vy = bodyQuat * glm::vec3(0, 1, 0);
        vz = bodyQuat * glm::vec3(0, 0, 1); //(UP)

        //find the "sensor" pos
        glm::vec3 centerpoint = offsetPos + ((vz * 0.15f) + (vy * 0.15f) + (vx * 0.15f));

        //drawCube({ offsetPos.x, offsetPos.y, offsetPos.z }, 0.10f, red);
        //drawCube({ centerpoint.x, centerpoint.y, centerpoint.z }, 0.05f, red);

        deadCameraframes = 30;

        //glm::quat cameraQuat = *(glm::quat*)(&camera.body->Rotation);
        glm::vec3 cameraUp = bodyQuat * glm::vec3(0, 0, 1);;

        camera::drawCameraWindow(fps);

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
        camera::interlacedImage(frameBuffer, toolgun::cameraResolution, flip, fov, 1.f, &bodyQuat, { centerpoint.x, centerpoint.y, centerpoint.z }, { -1, 0, 0 }, { cameraUp.x, cameraUp.y, cameraUp.z }, &rcf);
        camera::constructFrameManual(frameBuffer, toolgun::cameraResolution, false);

        FRAMEEND = execTimer.now();
        auto exTime = FRAMEEND - FRAMESTART;
        fps = (std::chrono::duration_cast<std::chrono::microseconds>(exTime).count() / 1000.f);
	}
}
