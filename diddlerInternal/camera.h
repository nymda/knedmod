#pragma once

namespace camera {
	void updateCameraFrame(float* pixels, int resolution, float min, float max, bool saveSnapshot);
	void drawCameraWindow();
	void quatCameraOutline(int resolution, float iFov);
	void updateCameraFrameColor(byte* pixels, int resolution, float min, float max, bool saveSnapshot);
}
