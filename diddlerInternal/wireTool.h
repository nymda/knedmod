#pragma once
#include "wireObjectSpawn.h"

namespace wireObjects {

	enum class wireToolSetting {
		WTS_Interact,
		WTS_Place
	};
	extern wireToolSetting toolgunSetting;
	extern wireObjectName toolgunSelectedObject;
	void execTool();
}