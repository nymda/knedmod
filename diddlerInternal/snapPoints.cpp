#include "snapPoints.h"
#include "drawCube.h"

namespace snapPoints {
	void drawSnapPoints(TDShape* shape) {
		td::Color white{ 1.f, 1.f, 1.f, 1.f };
		td::Color red{ 1.f, 0.f, 0.f, 1.f };

		if (!shape) { return; }
		if (shape->Type != entityType::Shape) { return; }

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

		drawCube(math::v3_glm2td(targetShapeWorldPosition), 0.05f, white);
		drawCube(math::v3_glm2td(targetShapeCenterPosition), 0.05f, red);
	}

	std::vector<glm::vec3> getSnapPoints(TDShape* shape) {

	}

}