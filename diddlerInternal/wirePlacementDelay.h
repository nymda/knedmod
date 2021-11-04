#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementDelay : public wireObj {
	public:
		bool previousInputState = false;
		int stateChangeCountdown = 0;
		int countedFrames = 0;
		bool counting = false;

		wirePlacementDelay();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}