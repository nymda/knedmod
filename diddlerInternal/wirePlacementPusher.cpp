#include "wireObjectBase.h"
#include "wirePlacementPusher.h"
#include "dotProjector.h"
#include "drawCube.h"

namespace wireObjects {
	wirePlacementPusher::wirePlacementPusher() {

	}

	DWORD wirePlacementPusher::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.45f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Green, { 0.45f, 0.35f, 0.05f });
		this->nodes.push_back(_green);

		this->rcf.m_IgnoredShapes.push_back(this->housing);

		this->memory = 200;

		return 0x01;
	}

	DWORD wirePlacementPusher::exec() {
		if ((bool)(nodes[0]->getValue())) {
			td::Color red = { 1.f, 0.f, 0.f, 1.f };

			glm::quat targetShapeWorldrotation = math::expandRotation(math::q_td2glm(housing->getParentBody()->Rotation), math::q_td2glm(housing->rOffset));
			glm::vec3 targetShapeWorldPosition = math::expandPosition(math::q_td2glm(housing->getParentBody()->Rotation), math::v3_td2glm(housing->getParentBody()->Position), math::v3_td2glm(housing->pOffset));

			glm::vec3 vx = targetShapeWorldrotation * glm::vec3(1, 0, 0);
			glm::vec3 vy = targetShapeWorldrotation * glm::vec3(0, 1, 0);
			glm::vec3 vz = targetShapeWorldrotation * glm::vec3(0, 0, 1); //(UP)

			glm::vec3 thrusterLocation = targetShapeWorldPosition + ((vz * 0.35f) + (vy * 0.25f) + (vx * 0.25f));

			float 	power = (float)nodes[1]->getValue();

			this->housing->Intergrity = power / 100.f;

			dotProjector::pixelResponse* pixelResponse = dotProjector::projectDotMatrix((float)(this->memory / 40), (float)this->memory / 100.f, 1.f, true, &targetShapeWorldrotation, math::v3_glm2td(thrusterLocation), { 0, 0, 1 }, { 0, 1, 0 }, &rcf);

			for (int i = 0; i < pixelResponse->size; i++) {
				if (pixelResponse->data[i].RCSuccessful) {
					glm::vec3 forceVector = math::v3_td2glm(pixelResponse->data[i].dirVec) * power;
					glb::tdApplyForce(pixelResponse->data[i].hitshape->getParentBody(), &thrusterLocation, &forceVector, power);
					drawCube(pixelResponse->data[i].worldPos, 0.05f, red);
				}
			}
		}
		else {
			this->housing->Intergrity = 0.f;
		}

		return 0x01;
	}

	DWORD wirePlacementPusher::usrExec() {
		return 0x01;
	}

}