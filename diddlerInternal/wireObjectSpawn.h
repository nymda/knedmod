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
		OBJ_PositionTracker
	};

	struct wireObjectInfo {
		wireObjectName name;
		const char* path;
		TDVox* voxTemplate;
	};

	static wireObjectInfo validWireObjects[] = {
		{wireObjectName::OBJ_IntBus, "KM_Misc\\KM_Wire\\w_bus\\object.vox", 0},
		{wireObjectName::OBJ_BoolBus, "KM_Misc\\KM_Wire\\w_bus\\object.vox", 0},
		{wireObjectName::OBJ_ConstantValue, "KM_Misc\\KM_Wire\\w_constantValue\\object.vox", 0},
		{wireObjectName::OBJ_Explosive, "KM_Misc\\KM_Wire\\w_explosive\\object.vox", 0},
		{wireObjectName::OBJ_GreaterThan, "KM_Misc\\KM_Wire\\w_greaterThan\\object.vox", 0},
		{wireObjectName::OBJ_Lamp, "KM_Misc\\KM_Wire\\w_lamp\\object.vox", 0},
		{wireObjectName::OBJ_LessThan, "KM_Misc\\KM_Wire\\w_lessThan\\object.vox", 0},
		{wireObjectName::OBJ_Raycast, "KM_Misc\\KM_Wire\\w_raycaster\\object.vox", 0},
		{wireObjectName::OBJ_Button, "KM_Misc\\KM_Wire\\w_button\\object.vox", 0},
		{wireObjectName::OBJ_BalloonDeployer, "KM_Misc\\KM_Wire\\w_balloonDeployer\\object.vox", 0},
		{wireObjectName::OBJ_ANDgate, "KM_Misc\\KM_Wire\\w_ANDgate\\object.vox", 0},
		{wireObjectName::OBJ_ORgate, "KM_Misc\\KM_Wire\\w_ORgate\\object.vox", 0},
		{wireObjectName::OBJ_NOTgate, "KM_Misc\\KM_Wire\\w_NOTgate\\object.vox", 0},
		{wireObjectName::OBJ_PositionTracker, "KM_Misc\\KM_Wire\\w_gps\\object.vox", 0},
	};

	wireObjectInfo findByName(wireObjectName name);

	//wireobjects are always spawned using the toolgun and always get placed on a target object
	wireObj* spawnWireObject(wireObjectName name);
}