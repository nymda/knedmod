#pragma once
#include "tgtBase.h"
struct tgt_attribute : public tgt {
	tgt_attribute();
	DWORD exec() override;
};