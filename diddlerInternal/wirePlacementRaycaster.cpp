#pragma once
#include "wireObjectBase.h"
#include "wirePlacementRaycaster.h"
#include "Raycaster.h"
#include "drawCube.h"

namespace wireObjects {

	wirePlacementRaycaster::wirePlacementRaycaster() {

	}

	DWORD wirePlacementRaycaster::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Red, { 0.45f, 0.25f, 0.05f });
		this->nodes.push_back(_red);

		this->rcf.m_IgnoredShapes.push_back(this->housing);

		return 0x01;
	};

	DWORD wirePlacementRaycaster::exec() {
		glm::quat targetShapeWorldrotation = math::expandRotation(math::q_td2glm(housing->getParentBody()->Rotation), math::q_td2glm(housing->rOffset));
		glm::vec3 targetShapeWorldPosition = math::expandPosition(math::q_td2glm(housing->getParentBody()->Rotation), math::v3_td2glm(housing->getParentBody()->Position), math::v3_td2glm(housing->pOffset));
		
		glm::vec3 vx = targetShapeWorldrotation * glm::vec3(1, 0, 0);
		glm::vec3 vy = targetShapeWorldrotation * glm::vec3(0, 1, 0);
		glm::vec3 vz = targetShapeWorldrotation * glm::vec3(0, 0, 1); //(UP)

		glm::vec3 rayCastPosition = targetShapeWorldPosition + ((vz * 0.25f) + (vy * 0.25f) + (vx * 0.25f));
		
		raycaster::rayData rd = raycaster::castRayManual(math::v3_glm2td(rayCastPosition), math::v3_glm2td(vz), &this->rcf);

		nodes[0]->setValue(convFloatToInt(rd.distance));


		//glm::vec3 hhhh = rayCastPosition + ((vz * 2.f) + (vy * 0.f) + (vx * 0.f));

		//drawCube(rd.worldPos, 0.1f, nodeColourActive[0]);

		return 0x01;
	};

	DWORD wirePlacementRaycaster::usrExec() {

		return 0x01;
	};
	
}