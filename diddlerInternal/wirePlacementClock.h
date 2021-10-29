#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementClock : public wireObj {
	public:
		int onTimeCountdown;

		wirePlacementClock();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}