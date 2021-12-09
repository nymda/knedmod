#pragma once
#include "tgtBase.h"

struct tgt_minigun : public tgt {

	int bulletCount = 5;
	float minigunFov = 2.f;
	int bulletType = 0;
	float u1 = 100.f;
	int u2 = 100;

	tgt_minigun();
	DWORD exec() override;
};