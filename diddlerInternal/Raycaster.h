#pragma once
#include "types.h"
#include "Global.h"

namespace raycaster {

	struct rayData {
		float distance;
		td::Vec3 worldPos;
		td::Vec3 angle;
		TDShape* hitShape;
	};

	rayData castRayManual(td::Vec3 position, td::Vec3 rotation, RaycastFilter* filter);
	rayData castRayPlayer();

}

