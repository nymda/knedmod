#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementToggle : public wireObj {
	public:
		bool previousInputState = false;
		wirePlacementToggle();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}