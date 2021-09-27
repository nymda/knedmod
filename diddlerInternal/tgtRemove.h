#pragma once
#include "tgtBase.h"

struct tgt_remove : public tgt {
	tgt_remove();
	DWORD exec() override;
};