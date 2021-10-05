#include "snapPoints.h"
#include "drawCube.h"

namespace snapPoints {

	bool pointNearPoint(glm::vec3 pointA, glm::vec3 pointB, float maxDist) {
		float dist = sqrt(pow(pointA.x - pointB.x, 2) + pow(pointA.y - pointB.y, 2) + pow(pointA.z - pointB.z, 2));
		if (dist < maxDist) {
			return true;
		}
		return false;
	}

	bool getClosestSnapPoint(glm::vec3 sourcePoint, std::vector<glm::vec3> snapPoints, float maxDist, glm::vec3* out) {
		bool hasSnapPoint = false;
		glm::vec3 closest = { 0, 0, 0 };
		float closestDist = INT32_MAX;
		for (glm::vec3 cVec3 : snapPoints) {
			float dist = sqrt(pow(sourcePoint.x - cVec3.x, 2) + pow(sourcePoint.y - cVec3.y, 2) + pow(sourcePoint.z - cVec3.z, 2));
			if (dist < closestDist && dist < maxDist) {
				closestDist = dist;
				closest = cVec3;
				hasSnapPoint = true;
			}
		}

		if (hasSnapPoint) {
			*out = closest;
			return true;
		}
		else {
			return false;
		}
	}

	void drawSnapPoints(TDShape* shape) {
		td::Color green{ 0.f, 1.f, 0.f, 1.f };

		std::vector<glm::vec3> points = getSnapPoints(shape);
		for (glm::vec3 v3 : points) {
			drawCube(math::v3_glm2td(v3), 0.05f, green);
		}
	}

	std::vector<glm::vec3> getSnapPoints(TDShape* shape) {
		static std::vector<glm::vec3> response = {};

		if (!shape) { return response; }
		if (shape->Type != entityType::Shape) { return response; }

		float voxSizeX = (shape->pVox->sizeX / 10.f);
		float voxSizeY = (shape->pVox->sizeY / 10.f);
		float voxSizeZ = (shape->pVox->sizeZ / 10.f);

		glm::quat targetShapeWorldrotation = math::expandRotation(math::q_td2glm(shape->getParentBody()->Rotation), math::q_td2glm(shape->rOffset));
		glm::vec3 targetShapeWorldPosition = math::expandPosition(math::q_td2glm(shape->getParentBody()->Rotation), math::v3_td2glm(shape->getParentBody()->Position), math::v3_td2glm(shape->pOffset));

		glm::vec3 vx = targetShapeWorldrotation * glm::vec3(-1, 0, 0);
		glm::vec3 vy = targetShapeWorldrotation * glm::vec3(0, -1, 0);
		glm::vec3 vz = targetShapeWorldrotation * glm::vec3(0, 0, -1); //(UP)

		glm::vec3 translationCenter = ((vx * (voxSizeX * 0.5f)) + (vy * (voxSizeY * 0.5f)) + (vz * (voxSizeZ * 0.5f)));
		glm::vec3 targetShapeCenterPosition = targetShapeWorldPosition - translationCenter;

		//drawCube(math::v3_glm2td(targetShapeCenterPosition), 0.05f, red);

		glm::vec3 target0 = targetShapeCenterPosition - (vy * (voxSizeY * 0.5f));
		glm::vec3 target1 = targetShapeCenterPosition + (vy * (voxSizeY * 0.5f));

		glm::vec3 target2 = targetShapeCenterPosition - (vx * (voxSizeX * 0.5f));
		glm::vec3 target3 = targetShapeCenterPosition + (vx * (voxSizeX * 0.5f));

		glm::vec3 target4 = targetShapeCenterPosition - (vz * (voxSizeZ * 0.5f));
		glm::vec3 target5 = targetShapeCenterPosition + (vz * (voxSizeZ * 0.5f));

		response = { target0, target1, target2, target3, target4, target5 };
		return response;
	}

}