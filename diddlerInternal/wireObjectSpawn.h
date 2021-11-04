#pragma once
#include "wireObjectBase.h"
#include "wirePlacements.h"
#include <vector>

namespace wireObjects {

	static enum class wireObjectName : int {
		OBJ_IntBus = 0,
		OBJ_BoolBus = 1,
		OBJ_ConstantValue = 2,
		OBJ_Explosive = 3,
		OBJ_GreaterThan = 4,
		OBJ_Lamp = 5,
		OBJ_LessThan = 6,
		OBJ_Raycast = 7,
		OBJ_Button = 8,
		OBJ_BalloonDeployer = 9,
		OBJ_ANDgate = 10,
		OBJ_ORgate = 11,
		OBJ_NOTgate = 12,
		OBJ_PositionTracker = 13,
		OBJ_RadioTx = 14,
		OBJ_RadioRx = 15,
		OBJ_Clock = 16,
		OBJ_Delay = 17
	};

	struct wireObjectInfo {
		wireObjectName name;
		const char* path;
		TDVox* voxTemplate;
	};

	extern wireObjectInfo validWireObjects[18];

	wireObjectInfo findByName(wireObjectName name);

	//wireobjects are always spawned using the toolgun and always get placed on a target object
	wireObj* spawnWireObject(wireObjectName name);
}