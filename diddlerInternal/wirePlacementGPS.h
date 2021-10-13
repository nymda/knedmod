#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementGPS : public wireObj {
	public:
		wirePlacementGPS();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}