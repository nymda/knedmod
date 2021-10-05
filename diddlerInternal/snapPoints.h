#pragma once
//#include "TDObjects.h"
#include "Global.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

namespace snapPoints {
	bool pointNearPoint(glm::vec3 pointA, glm::vec3 pointB, float maxDist);
	bool getClosestSnapPoint(glm::vec3 sourcePoint, std::vector<glm::vec3> snapPoints, float maxDist, glm::vec3* out);
	void drawSnapPoints(TDShape* shape);
	std::vector<glm::vec3> getSnapPoints(TDShape* shape);
}