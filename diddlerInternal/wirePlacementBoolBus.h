#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementBoolBus : public wireObj {
	public:
		wirePlacementBoolBus();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}