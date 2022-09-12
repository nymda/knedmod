#pragma once
#include "TDObjects.h"

namespace highlightManager {
	void pushShapeOutline(TDShape* shape, td::Color colour, int duration);
	void pushShapeHighlight(TDShape* shape, int duration);
	void update();
}