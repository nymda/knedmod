#pragma once
#include "tgtBase.h"

struct tgt_slice: public tgt {
	tgt_slice();
	DWORD exec() override;
};