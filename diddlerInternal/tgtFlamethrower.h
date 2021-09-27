#pragma once
#include "tgtBase.h"

struct tgt_flamethrower : public tgt {
	tgt_flamethrower();
	DWORD exec() override;
};