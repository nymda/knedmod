#pragma once
#include "wireObjectBase.h"
#include "objectSpawner.h"

namespace wireObjects {
	class wirePlacementThruster : public wireObj {
	public:
		wirePlacementThruster();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}