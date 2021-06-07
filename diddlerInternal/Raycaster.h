#pragma once
#include "types.h"
#include "Global.h"

namespace raycaster {

	struct palInfo {
		byte data[128] = {};
	};

	struct rayData {
		float distance;
		td::Vec3 worldPos;
		td::Vec3 angle;
		TDShape* hitShape;
		uintptr_t* palette;
	};

	rayData castRayManual(td::Vec3 position, td::Vec3 rotation, RaycastFilter* filterCus, td::VoxelsPaletteInfo* voxInfo);
	rayData castRayPlayer();

}

