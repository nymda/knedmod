#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementLamp : public wireObj {
	public:
		wirePlacementLamp();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}