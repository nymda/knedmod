#pragma once
#include "wireObjectBase.h"
#include "wirePlacements.h"
#include <vector>

#define W_OBJECT_COUNT 23

namespace wireObjects {

	struct wireObjectInfo {
		wireObjectName name;
		const char* path;
		TDVox* voxTemplate;
	};

	extern wireObjectInfo validWireObjects[W_OBJECT_COUNT];

	wireObjectInfo findByName(wireObjectName name);

	//wireobjects are always spawned using the toolgun and always get placed on a target object
	wireObj* spawnWireObject(wireObjectName name);
}