#pragma once
#include "wireObjectSpawn.h"

namespace wireObjects {
	extern int targetUserMemory;

	enum class wireToolSetting {
		WTS_Interact,
		WTS_Place,
		WTS_SetUserMemory
	};
	extern wireToolSetting toolgunSetting;
	extern wireObjectName toolgunSelectedObject;
	void execTool();
}