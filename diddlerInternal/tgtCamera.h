#pragma once
#include "tgtBase.h"
#include "threadedCamera.h"

struct tgt_camera : public tgt {
	threadCamera::KMCamera* playerCamera = 0;
	int resolutionX = 100;
	int resolutionY = 100;
	float fov = 8.f;

	tgt_camera();
	DWORD exec() override;
	void setResolution();
};