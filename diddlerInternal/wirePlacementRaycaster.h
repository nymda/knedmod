#pragma once
#include "wireObjectBase.h"
#include "Raycaster.h"

namespace wireObjects {
	class wirePlacementRaycaster : public wireObj {
	public:
		RaycastFilter rcf = {};

		wirePlacementRaycaster();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}