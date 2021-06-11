#pragma once
#include "dotProjector.h"

namespace camera {
	extern bool colourMode;
	extern bool mono;
	extern bool interlaceMode;
	extern bool transparency;

	//void updateCameraFrame(float* pixels, int resolution, float min, float max, bool saveSnapshot);
	void drawCameraWindow();
	void constructDistanceFrame(dotProjector::pixelResponse* pixelResponse, int resolution, float min, float max);
	void constructColourFrame(dotProjector::pixelResponse* pixelResponse, int resolution, bool enableDistanceFog);
	void updateImageColour(int resolution, float fov);
	void constructFrameManual(byte* pixels, int resolution, bool saveSnapshot);
	void interlacedImage(byte* frameBuffer, int resolution, bool flip, float fov, float aspect, glm::quat* camRotation, td::Vec3 camPosition, td::Vec3 forwardVector, td::Vec3 upVector, RaycastFilter* filter);

}
