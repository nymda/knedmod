#include "wireObjectBase.h"
#include "wirePlacementThruster.h"

namespace wireObjects {

	wirePlacementThruster::wirePlacementThruster() {

	}

	DWORD wirePlacementThruster::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.35f, 0.05f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Green, { 0.35f, 0.25f, 0.05f });
		this->nodes.push_back(_green);

		this->memory = 25.f;

		return 0x01;
	}

	DWORD wirePlacementThruster::exec() {

		if ((bool)(nodes[0]->getValue())) {
			glm::quat targetShapeWorldrotation = math::expandRotation(math::q_td2glm(housing->getParentBody()->Rotation), math::q_td2glm(housing->rOffset));
			glm::vec3 targetShapeWorldPosition = math::expandPosition(math::q_td2glm(housing->getParentBody()->Rotation), math::v3_td2glm(housing->getParentBody()->Position), math::v3_td2glm(housing->pOffset));

			glm::vec3 vx = targetShapeWorldrotation * glm::vec3(1, 0, 0);
			glm::vec3 vy = targetShapeWorldrotation * glm::vec3(0, 1, 0);
			glm::vec3 vz = targetShapeWorldrotation * glm::vec3(0, 0, 1); //(UP)

			glm::vec3 thrusterLocation = targetShapeWorldPosition + ((vz * 0.15f) + (vy * 0.15f) + (vx * 0.15f));

			float power = 0.f;
			if (nodes[1]->isConnected()) {
				power = (float)nodes[1]->getValue();
			}
			else {
				power = (float)this->memory;
			}

			glm::vec3 forceVector = (-vz) * power;
			this->housing->Intergrity = power / 100.f;

			glb::tdApplyForce(this->housing->getParentBody(), &thrusterLocation, &forceVector, power);
		}
		else {
			this->housing->Intergrity = 0.f;
		}

		return 0x01;
	}

	DWORD wirePlacementThruster::usrExec() {

		return 0x01;
	}
}