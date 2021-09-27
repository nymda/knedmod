#pragma once
//#include "TDObjects.h"
#include "Global.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

namespace snapPoints {
	void drawSnapPoints(TDShape* shape);
	std::vector<glm::vec3> getSnapPoints(TDShape* shape);
}