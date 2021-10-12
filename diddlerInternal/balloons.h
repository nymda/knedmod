#pragma once
#include <vector>
#include "Global.h"

namespace balloons {
	struct balloon {
		TDShape* shape;
		TDBody* body;
		float upVelocity;

		bool destroyed = false;
		bool hasString = false;
		int wait = 1;
		td::Vec3 origin = { 0, 0, 0 };
	};

	void updateAllBalloons();
	void spawnBalloon(float power);
	extern std::vector<balloon> balloons;
}