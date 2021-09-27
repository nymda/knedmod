#pragma once
#include "tgtBase.h"

struct tgt_dev : public tgt {
	tgt_dev();
	DWORD exec() override;
};