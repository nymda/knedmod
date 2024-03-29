#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementTemplate : public wireObj {
	public:
		wirePlacementTemplate();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}