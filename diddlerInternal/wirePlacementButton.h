#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementButton : public wireObj {
	public:
		wirePlacementButton();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}