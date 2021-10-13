#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementConstInt : public wireObj {
	public:
		wirePlacementConstInt();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}