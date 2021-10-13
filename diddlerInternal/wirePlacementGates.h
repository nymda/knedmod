#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementAND : public wireObj {
	public:
		wirePlacementAND();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};

	class wirePlacementOR : public wireObj {
	public:
		wirePlacementOR();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};


	class wirePlacementNOT : public wireObj {
	public:
		wirePlacementNOT();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
		DWORD usrExec() override;
	};
}