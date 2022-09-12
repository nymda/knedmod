#include "wireObjectBase.h"
#include "wirePlacementPower.h"

namespace wireObjects {
	wirePlacementPower::wirePlacementPower() {

	}

	DWORD wirePlacementPower::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red0 = new wireNode();
		_red0->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.05f, 0.15f, 0.05f });
		this->nodes.push_back(_red0);

		wireNode* _red1 = new wireNode();
		_red1->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.15f, 0.05f, 0.05f });
		this->nodes.push_back(_red1);

		wireNode* _red2 = new wireNode();
		_red2->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.15f, 0.25f, 0.05f });
		this->nodes.push_back(_red2);

		wireNode* _red3 = new wireNode();
		_red3->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.25f, 0.15f, 0.05f });
		this->nodes.push_back(_red3);

		return 0x01;
	}

	DWORD wirePlacementPower::exec() {

		this->nodes[0]->setValue(1);
		this->nodes[1]->setValue(1);
		this->nodes[2]->setValue(1);
		this->nodes[3]->setValue(1);

		return 0x01;
	}

	DWORD wirePlacementPower::usrExec() {
		return 0x01;
	}

}