#include "wireObjectBase.h"
#include "wirePlacementButton.h"

namespace wireObjects {

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

		if (this->onTimeCountdown > 0) {
			this->onTimeCountdown--;
			this->nodes[0]->setValue(1);
			this->housing->Intergrity = 1.f;
		}
		else {
			this->nodes[0]->setValue(0);
			this->housing->Intergrity = 0.f;
		}

		this->memory = this->onTimeCountdown;

		return 0x01;
	}

	DWORD wirePlacementButton::usrExec() {
		printf_s("exec called: %p\n", this);

		if (this->onTimeCountdown == 0) {
			this->onTimeCountdown = 30;
		}
		return 0x01;
	}
}