#pragma once
#include "tgtBase.h"
#include "global.h"

struct tgt_wire : public tgt {
    tgt_wire();
    DWORD exec() override;
};