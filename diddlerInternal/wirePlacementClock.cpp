#include "wirePlacementClock.h"
#include "wireObjectBase.h"

namespace wireObjects {
	wirePlacementClock::wirePlacementClock() {

	}

	DWORD wirePlacementClock::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.25f, 0.25f, 0.05f });
		this->nodes.push_back(_red);

		return 0x01;
	}

	DWORD wirePlacementClock::exec() {
		if (this->onTimeCountdown > 0) {
			this->onTimeCountdown--;
		}
		else {
			this->onTimeCountdown = this->memory;
			this->nodes[0]->setValue((int)(!(bool)this->nodes[0]->getOwnValue()));
			if ((bool)this->nodes[0]->getOwnValue()) {
				this->housing->Intergrity = 1.f;
			}
			else {
				this->housing->Intergrity = 0.f;
			}
		}

		return 0x01;
	}

	DWORD wirePlacementClock::usrExec() {

		return 0x01;
	}
}