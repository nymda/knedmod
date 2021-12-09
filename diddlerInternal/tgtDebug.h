#pragma once
#include "tgtBase.h"
#include "global.h"
#include "imgui/imgui.h"

struct tgt_debug: public tgt {
	TDShape* dbgShape = 0;
	TDBody* dbgBody = 0;
	ImGuiWindowFlags infoBoxFlags;

	tgt_debug();
	DWORD exec() override;
};