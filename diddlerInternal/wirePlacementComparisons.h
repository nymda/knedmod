#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementGreaterthan : public wireObj {
	public:
		wirePlacementGreaterthan();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};

	class wirePlacementLessthan : public wireObj {
	public:
		wirePlacementLessthan();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};

}