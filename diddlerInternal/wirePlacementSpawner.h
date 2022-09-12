#pragma once
#include "wireObjectBase.h"
#include "objectSpawner.h"

namespace wireObjects {
	class wirePlacementSpawner : public wireObj {
	public:
		bool previousInputState = false;
		spawner::LoadedSpawnableObject object;
		RaycastFilter rcf = {};

		wirePlacementSpawner();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}