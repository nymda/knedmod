#pragma once
#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "drawCube.h"

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
		int resolutionX = 100;
		int resolutionY = 75;
		int nextResolutionX = 100;
		int nextResolutionY = 75;
		bool bufferUpdateNeeded = false;
		void* camTexture;
		float lastFrameExecTime = 0.f;
		bool switchDestroyBuffers = false;

	public:
		bool cameraActive = true;
		bool cameraDestroyed = false;
		int DCF = 0;

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
		void setFov(float fov);
		float updateImage();

		float getFps();
		float getLastFrameTime();

		void destroy();
		bool isDestroyed();

	};

	void drawCameraWndw(KMCamera* camera);

	extern std::vector<KMCamera*> gameCameras;
}