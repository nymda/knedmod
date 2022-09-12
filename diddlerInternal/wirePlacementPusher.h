#pragma once
#include "wireObjectBase.h"
#include "Raycaster.h"

namespace wireObjects {
	class wirePlacementPusher : public wireObj {
	public:
		RaycastFilter rcf = {};

		wirePlacementPusher();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}