#pragma once
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
#include "dotProjector.h"
#include "camera.h"

namespace threadCamera {

	struct pixel {
		byte R = 0;
		byte G = 0;
		byte B = 0;
		byte A = 0;
	};

	class KMCamera {
		int resolutionX = 160;
		int resolutionY = 120;

		int nextResolutionX = 160;
		int nextResolutionY = 120;
		bool bufferUpdateNeeded = false;
		void* camTexture;
		float lastFrameExecTime = 0.f;
		bool cameraDestroyed = false;
		bool switchDestroyBuffers = false;

	public:
		bool cameraActive = true;

		RaycastFilter rcf = { };

		glm::quat rotation = { 0, 0, 0, 0 };
		glm::vec3 position = { 0, 0, 0 };
		glm::vec3 forward = { 0, 0, 0 };
		glm::vec3 up = { 0, 0, 0 };
		float fov = 8.f;

		pixel* bufferA = 0;
		pixel* bufferB = 0;
		pixel* bufferShow = bufferA;
		pixel* bufferWrite = bufferB;

		KMCamera(glm::quat rot, glm::vec3 pos, glm::vec3 forwv, glm::vec3 upv, int resX, int resY);
		void updateCameraSpecs(glm::quat rot, glm::vec3 pos, glm::vec3 forwv, glm::vec3 upv);
		void* createImage();
		void swapBuffers();
		void setResolution(int X, int Y);
		float updateImage();

		float getFps();
		float getLastFrameTime();

		void destroy();
		bool isDestroyed();

	};

	void drawCameraWndw(KMCamera* camera);

	extern std::vector<KMCamera*> gameCameras;
}