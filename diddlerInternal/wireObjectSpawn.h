#pragma once
#include "wireObjectBase.h"
#include "wirePlacements.h"
#include <vector>

namespace wireObjects {
	enum class wireObjectName {
		OBJ_IntBus,
		OBJ_BoolBus,
		OBJ_ConstantValue,
		OBJ_Explosive,
		OBJ_GreaterThan,
		OBJ_Lamp,
		OBJ_LessThan,
		OBJ_Raycast,
		OBJ_Button,
		OBJ_BalloonDeployer
	};

	struct wireObjectInfo {
		int id;
		wireObjectName name;
		const char* path;
		TDVox* voxTemplate;
	};

	const int wireObjectCount = 9;
	static wireObjectInfo validWireObjects[] = {
		{0, wireObjectName::OBJ_IntBus, "KM_Misc\\KM_Wire\\w_bus\\object.vox", 0},
		{1, wireObjectName::OBJ_BoolBus, "KM_Misc\\KM_Wire\\w_bus\\object.vox", 0},
		{2, wireObjectName::OBJ_ConstantValue, "KM_Misc\\KM_Wire\\w_constantValue\\object.vox", 0},
		{3, wireObjectName::OBJ_Explosive, "KM_Misc\\KM_Wire\\w_explosive\\object.vox", 0},
		{4, wireObjectName::OBJ_GreaterThan, "KM_Misc\\KM_Wire\\w_greaterThan\\object.vox", 0},
		{5, wireObjectName::OBJ_Lamp, "KM_Misc\\KM_Wire\\w_lamp\\object.vox", 0},
		{6, wireObjectName::OBJ_LessThan, "KM_Misc\\KM_Wire\\w_lessThan\\object.vox", 0},
		{7, wireObjectName::OBJ_Raycast, "KM_Misc\\KM_Wire\\w_raycaster\\object.vox", 0},
		{8, wireObjectName::OBJ_Button, "KM_Misc\\KM_Wire\\w_button\\object.vox", 0},
		{9, wireObjectName::OBJ_BalloonDeployer, "KM_Misc\\KM_Wire\\w_raycaster\\object.vox", 0},
	};

	//wireobjects are always spawned using the toolgun and always get placed on a target object
	wireObj* spawnWireObject(int objectID);
}