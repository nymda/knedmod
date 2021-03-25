#pragma once
#include "Global.h"
#include "mem.h"

namespace noclip {
	extern bool enabled;
	extern bool inNoclip;

	void backupInstructions();
	void ToggleNoclip();
	void update();
}