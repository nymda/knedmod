#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementBalloonDeployer : public wireObj {
	public:
		wirePlacementBalloonDeployer();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}