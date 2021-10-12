#include "wireObjectBase.h"
#include "wirePlacementButton.h"

namespace wireObjects {

	int onTimeCountdown = 0;

	wirePlacementButton::wirePlacementButton() {

	}

	DWORD wirePlacementButton::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.45f, 0.25f, 0.05f });
		this->nodes.push_back(_red);

		return 0x01;
	}

	DWORD wirePlacementButton::exec() {

		if (onTimeCountdown > 0) {
			onTimeCountdown--;
			this->nodes[0]->setValue(1);
		}
		else {
			this->nodes[0]->setValue(0);
		}

		this->memory = onTimeCountdown;

		return 0x01;
	}

	DWORD wirePlacementButton::usrExec() {
		if (onTimeCountdown == 0) {
			onTimeCountdown = 15;
		}
		return 0x01;
	}
}