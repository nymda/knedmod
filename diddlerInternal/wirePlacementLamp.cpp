#include "wireObjectBase.h"
#include "wirePlacementLamp.h"

namespace wireObjects {
	wirePlacementLamp::wirePlacementLamp() {

	}

	DWORD wirePlacementLamp::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.45f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		return 0x01;
	}

	DWORD wirePlacementLamp::exec() {
		if ((bool)nodes[0]->getValue()) {
			this->housing->Intergrity = 1.f;
		}
		else {
			this->housing->Intergrity = 0.f;
		}

		return 0x01;
	}

	DWORD wirePlacementLamp::usrExec() {
		return 0x01;
	}

}