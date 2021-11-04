#include "wireObjectBase.h"
#include "wirePlacementDelay.h"

namespace wireObjects {
	wirePlacementDelay::wirePlacementDelay() {

	}

	DWORD wirePlacementDelay::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.25f, 0.25f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Green, { 0.05f, 0.25f, 0.05f });
		this->nodes.push_back(_green);

		return 0x01;
	}

	//if red changes from low to high
	//begin counting frames since the change
	//if counted frames < delay time, wait the rest of the time

	DWORD wirePlacementDelay::exec() {
		if ((this->previousInputState != (bool)(nodes[0]->getValue())) && (bool)(nodes[0]->getValue())) {
			printf_s("1 Coutdown: %i\n", this->stateChangeCountdown);
			this->stateChangeCountdown = this->memory;
		}

		if (this->stateChangeCountdown > 0) {
			printf_s("2 Coutdown: %i\n", this->stateChangeCountdown);
			this->stateChangeCountdown--;
		}
		else if (this->stateChangeCountdown <= 0 && this->stateChangeCountdown > -60) {
			printf_s("3 Coutdown: %i\n", this->stateChangeCountdown);
			this->stateChangeCountdown--;
			nodes[1]->setValue(1);
		}
		else {
			nodes[1]->setValue(0);
		}

		this->previousInputState = (bool)(nodes[0]->getValue());

		return 0x01;
	}

	DWORD wirePlacementDelay::usrExec() {
		return 0x01;
	}

}