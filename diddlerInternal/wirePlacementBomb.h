#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementBomb : public wireObj {
	public:
		wirePlacementBomb();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}