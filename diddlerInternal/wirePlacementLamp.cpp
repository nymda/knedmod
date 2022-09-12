#include "wireObjectBase.h"
#include "wirePlacementLamp.h"

namespace wireObjects {
	wirePlacementLamp::wirePlacementLamp() {

	}

	DWORD wirePlacementLamp::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.45f, 0.05f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Green, { 0.45f, 0.25f, 0.05f });
		this->nodes.push_back(_green);

		this->memory = 100;

		return 0x01;
	}

	DWORD wirePlacementLamp::exec() {
		float power = 0.f;
		if (nodes[1]->isConnected()) {
			power = (float)nodes[1]->getValue() / 1000.f;
		}
		else {
			power = (float)this->memory / 1000.f;
		}

		if ((bool)nodes[0]->getValue()) {
			this->housing->Intergrity = power;
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