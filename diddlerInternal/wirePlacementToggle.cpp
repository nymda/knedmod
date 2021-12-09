#pragma once
#include "wireObjectBase.h"
#include "wirePlacementToggle.h"

namespace wireObjects {

	wirePlacementToggle::wirePlacementToggle() {

	}

	DWORD wirePlacementToggle::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.35f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Green, { 0.05f, 0.15f, 0.05f });
		this->nodes.push_back(_green);

		return 0x01;
	}

	DWORD wirePlacementToggle::exec() {
		if ((this->previousInputState != (bool)(nodes[0]->getValue())) && (bool)(nodes[0]->getValue())) {
			if ((bool)(nodes[1]->getOwnValue())) {
				this->housing->Intergrity = 0.f;
				nodes[1]->setValue(0);
			}
			else {
				this->housing->Intergrity = 1.f;
				nodes[1]->setValue(1);
			}
		}

		this->previousInputState = (bool)(nodes[0]->getValue());

		return 0x01;
	}

	DWORD wirePlacementToggle::usrExec() {

		return 0x01;
	}
}