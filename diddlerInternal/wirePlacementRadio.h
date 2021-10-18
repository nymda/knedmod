#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementRadioTx : public wireObj {
	public:
		wirePlacementRadioTx();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};

	class wirePlacementRadioRx : public wireObj {
	public:
		wirePlacementRadioRx();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}