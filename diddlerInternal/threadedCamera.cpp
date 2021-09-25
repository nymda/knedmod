#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "drawCube.h"
#include "toolgun.h"
#include "Global.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "dotProjector.h"
#include "camera.h"
#include "threadedCamera.h"

namespace threadCamera {

	std::vector<KMCamera*> gameCameras = {};

	float randFloat(float min, float max) {
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

	void drawCameraWndw(KMCamera* camera) {
		if (!camera) { return; }
		if (camera->isDestroyed()) { return; }

		ImGui::Begin("Camera");
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 posMin = ImGui::GetWindowContentRegionMin();
		ImVec2 posMax = ImGui::GetWindowContentRegionMax();
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 adjustedPosMin = ImVec2(windowPos.x + posMin.x, windowPos.y + posMin.y);
		ImVec2 adjustedPosMax = ImVec2(windowPos.x + posMax.x, windowPos.y + posMax.y);
		drawList->AddImage((void*)camera->createImage(), adjustedPosMin, adjustedPosMax);
		ImGui::Text("FPS: %.2f", camera->getFps());
		ImGui::End();
	}

	KMCamera::KMCamera(glm::quat rot, glm::vec3 pos, glm::vec3 forwv, glm::vec3 upv, int resX, int resY) {
		glGenTextures(1, (GLuint*)camTexture);
		resolutionX = resX;
		resolutionY = resY;

		bufferA = new pixel[resolutionX * resolutionY];
		bufferB = new pixel[resolutionX * resolutionY];
		bufferShow = bufferA;
		bufferWrite = bufferB;

		FillMemory(bufferA, (resolutionX * resolutionY) * 4, 0x00);
		FillMemory(bufferB, (resolutionX * resolutionY) * 4, 0x00);

		rotation = rot;
		position = pos;
		forward = forwv;
		up = upv;

		gameCameras.push_back(this);
	}

	void KMCamera::updateCameraSpecs(glm::quat rot, glm::vec3 pos, glm::vec3 forwv, glm::vec3 upv) {

		std::cout << "RW: " << std::to_string(rot.w) << " RX : " << std::to_string(rot.x) << " RY : " << std::to_string(rot.y) << " RZ : " << std::to_string(rot.z) << std::endl;
		std::cout << "PX : " << std::to_string(pos.x) << " PY : " << std::to_string(pos.y) << " PZ : " << std::to_string(pos.z) << std::endl;
		std::cout << "===" << std::endl;

		rotation = rot;
		position = pos;
		forward = forwv;
		up = upv;
	}

	void* KMCamera::createImage() {
		if (cameraDestroyed) {
			return 0;
		}

		glBindTexture(GL_TEXTURE_2D, (GLuint)camTexture);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolutionX, resolutionY, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)bufferShow);
		return camTexture;
	}

	void KMCamera::swapBuffers() {
		//std::cout << "SHOW: " << std::hex << bufferShow << " WRITE: " << std::hex << bufferWrite << std::endl;

		pixel* tmp = bufferShow;
		bufferShow = bufferWrite;
		bufferWrite = tmp;
	}

	void KMCamera::setResolution(int X, int Y) {
		nextResolutionX = X;
		nextResolutionY = Y;
		bufferUpdateNeeded = true;
	}

	std::chrono::high_resolution_clock execTimer;
	std::chrono::steady_clock::time_point FRAMESTART;
	std::chrono::steady_clock::time_point FRAMEEND;

	float KMCamera::getLastFrameTime() {
		return lastFrameExecTime;
	}

	float KMCamera::getFps() {
		return (1000.f / lastFrameExecTime);
	}

	void KMCamera::destroy() {
		switchDestroyBuffers = true;
	}
	
	bool KMCamera::isDestroyed() {
		return cameraDestroyed;
	}

	float KMCamera::updateImage() {
		if (switchDestroyBuffers) {
			delete(bufferA);
			delete(bufferB);
			cameraDestroyed = true;
		}

		glm::quat thisFrameQuat = rotation;
		glm::vec3 thisFramePosition = position;

		float pxSizeX = (fov / resolutionX);
		float pxSizeY = (fov / resolutionY);
		int pxPointer = 0;

		if (cameraDestroyed) {
			//bufferShow = bufferA;
			//for (int y = 0; y < resolutionY; y++) {
			//	byte cShade = (byte)(rand() % 255);

			//	for (int x = resolutionX; x > 0; x--) {
			//		bufferWrite[pxPointer] = { (byte)cShade,(byte)cShade, (byte)cShade, 0xFF };
			//		pxPointer++;
			//	}
			//}
			return 0.f;
		}

		if (bufferUpdateNeeded) {
			resolutionX = nextResolutionX;
			resolutionY = nextResolutionX;
			bufferUpdateNeeded = false;

			delete(bufferA);
			delete(bufferB);
			bufferA = new pixel[resolutionX * resolutionY];
			bufferB = new pixel[resolutionX * resolutionY];
			bufferShow = bufferA;
			bufferWrite = bufferB;

			FillMemory(bufferA, (resolutionX * resolutionY) * 4, 0x00);
			FillMemory(bufferB, (resolutionX * resolutionY) * 4, 0x00);
		}

		FRAMESTART = execTimer.now();
		glm::vec3 raycast_dir_bl = thisFrameQuat * forward;
		raycaster::rayData rd = raycaster::castRayManual(math::v3_glm2td(thisFramePosition), math::v3_glm2td(raycast_dir_bl), &rcf);
		glm::vec3 glTarget = glm::vec3(rd.worldPos.x, rd.worldPos.y, rd.worldPos.z);
		glm::mat4x4 vmatrix = glm::lookAt(thisFramePosition, glTarget, up);
		glm::mat4x4 pmatrix = glm::perspective(50.f, 1.f, 1.f, 1000.f);
		glm::mat4 invProjMat = glm::inverse(pmatrix);
		glm::mat4 invViewMat = glm::inverse(vmatrix);

		for (int y = 0; y < resolutionY; y++) {
			for (int x = resolutionX; x > 0; x--) {

				float comX = (fov / 2.f) - (x * pxSizeX) + randFloat(-(pxSizeX / 3.f), (pxSizeX / 3.f));
				float comY = (fov / 2.f) - (y * pxSizeY) + randFloat(-(pxSizeY / 3.f), (pxSizeY / 3.f));

				glm::vec2 ray_nds = glm::vec2(comX, comY);
				glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
				glm::vec4 eyeCoords = invProjMat * ray_clip;
				eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

				glm::vec4 rayWorld = invViewMat * eyeCoords;
				glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
				rd = raycaster::castRayManual(math::v3_glm2td(thisFramePosition), math::v3_glm2td(rayDirection), &rcf);
				int iThisDist = (rd.distance) / 2;
				int iColourR = ((rd.palette.m_Color.m_R * 255) - iThisDist);
				if (iColourR < 0) { iColourR = 0; }
				int iColourG = ((rd.palette.m_Color.m_G * 255) - iThisDist);
				if (iColourG < 0) { iColourG = 0; }
				int iColourB = ((rd.palette.m_Color.m_B * 255) - iThisDist);
				if (iColourB < 0) { iColourB = 0; }

				bufferWrite[pxPointer] = { (byte)iColourR,(byte)iColourG, (byte)iColourB, 0xFF };

				pxPointer++;
			}
		}
		swapBuffers();
		FRAMEEND = execTimer.now();
		lastFrameExecTime = (std::chrono::duration_cast<std::chrono::microseconds>(FRAMEEND - FRAMESTART).count() / 1000.f);
		return lastFrameExecTime;
	}

}