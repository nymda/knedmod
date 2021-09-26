#pragma once
#include <string>
#include "windows.h"

enum toolnames {
	TOOL_SPAWNER,
	TOOL_MINIGUN,
	TOOL_CLICKEXPLODE,
	TOOL_FLAMETHROWER,
	TOOL_DAMAGE,
	TOOL_REMOVE,
	TOOL_ATTRIBUTE,
	TOOL_LEAFBLOWER,
	TOOL_SLICE,
	TOOL_CAMERA,
	TOOL_ROPE,
	TOOL_WELD,
	TOOL_DEBUG,
	TOOL_TESTING
};

struct tgt {
	toolnames tool;
	virtual DWORD exec() = 0;
};
