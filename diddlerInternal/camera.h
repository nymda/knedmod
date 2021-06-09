#pragma once
#include "dotProjector.h"

namespace camera {
	extern bool colourMode;

	//void updateCameraFrame(float* pixels, int resolution, float min, float max, bool saveSnapshot);
	void drawCameraWindow();
	void constructDistanceFrame(dotProjector::pixelResponse* pixelResponse, int resolution, float min, float max);
	void constructColourFrame(dotProjector::pixelResponse* pixelResponse, int resolution, bool enableDistanceFog);
	void updateImageColour(int resolution, float fov);
	void constructFrameManual(byte* pixels, int resolution, bool saveSnapshot);

}
