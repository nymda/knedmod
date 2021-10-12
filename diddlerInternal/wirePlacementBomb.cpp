#include "wireObjectBase.h"
#include "wirePlacementBomb.h"

namespace wireObjects {

	wirePlacementBomb::wirePlacementBomb() {

	}

	DWORD wirePlacementBomb::init(TDShape* housing, int memoryVal) {

		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.55f, 0.25f, 0.05f });
		this->nodes.push_back(_red);

		return 0x01;
	}

	DWORD wirePlacementBomb::exec() {

		if ((bool)this->nodes[0]->getValue()) {
			td::Vec3 objectMin = this->housing->posMin;
			td::Vec3 objectMax = this->housing->posMax;
			td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
			glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, 2.f);
			this->softDispose();
		}

		return 0x01;
	}

	DWORD wirePlacementBomb::usrExec() {

		return 0x01;
	}
}