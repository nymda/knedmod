#pragma once
#include "tgtBase.h"

struct tgt_damage : public tgt {
	tgt_damage();
	DWORD exec() override;
};