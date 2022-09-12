#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementPower : public wireObj {
	public:
		wirePlacementPower();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}