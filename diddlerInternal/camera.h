#pragma once
#include "dotProjector.h"

namespace camera {
	enum class cameraMode : int {
		fullframe = 0,
		interlaced = 1,
		staged = 2,
	};

	extern cameraMode mode;
	extern bool colourMode;
	extern bool mono;
	extern bool transparency;
	extern bool staged_newFrame;
	extern bool showImageProgress;
	extern int staged_maxPixelsPerFrame;

	//void updateCameraFrame(float* pixels, int resolution, float min, float max, bool saveSnapshot);
	void drawCameraWindow();
	void constructDistanceFrame(dotProjector::pixelResponse* pixelResponse, int resolution, float min, float max);
	void constructColourFrame(dotProjector::pixelResponse* pixelResponse, int resolution, bool enableDistanceFog);
	void updateImageColour(int resolution, float fov);
	void constructFrameManual(byte* pixels, int resolution, bool saveSnapshot);
	void interlacedImage(byte* frameBuffer, int resolution, bool flip, float fov, float aspect, glm::quat* camRotation, td::Vec3 camPosition, td::Vec3 forwardVector, td::Vec3 upVector, RaycastFilter* filter);

}
