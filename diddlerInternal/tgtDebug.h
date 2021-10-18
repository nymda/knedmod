#pragma once
#include "tgtBase.h"
#include "global.h"
struct tgt_debug: public tgt {
	TDShape* dbgShape = 0;
	TDBody* dbgBody = 0;

	tgt_debug();
	DWORD exec() override;
};