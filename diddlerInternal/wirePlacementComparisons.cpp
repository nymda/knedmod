#include "wireObjectBase.h"
#include "wirePlacementComparisons.h"

namespace wireObjects{

	wirePlacementGreaterthan::wirePlacementGreaterthan() {

	}

	DWORD wirePlacementGreaterthan::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Red, { 0.25f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _blue = new wireNode();
		_blue->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Blue, { 0.05f, 0.25f, 0.05f });
		this->nodes.push_back(_blue);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Green, { 0.25f, 0.35f, 0.05f });
		this->nodes.push_back(_green);

		return 0x01;
	}

	DWORD wirePlacementGreaterthan::exec() {
		if (nodes[0]->getValue() > nodes[2]->getValue()) {
			nodes[1]->setValue(1);
		}
		else {
			nodes[1]->setValue(0);
		}
		return 0x01;
	}

	DWORD wirePlacementGreaterthan::usrExec() {
		return 0x01;
	}
	

	wirePlacementLessthan::wirePlacementLessthan() {

	}

	DWORD wirePlacementLessthan::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Red, { 0.25f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _blue = new wireNode();
		_blue->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Blue, { 0.05f, 0.25f, 0.05f });
		this->nodes.push_back(_blue);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Green, { 0.25f, 0.35f, 0.05f });
		this->nodes.push_back(_green);

		return 0x01;
	}

	DWORD wirePlacementLessthan::exec() {
		if (nodes[0]->getValue() < nodes[2]->getValue()) {
			nodes[1]->setValue(1);
		}
		else {
			nodes[1]->setValue(0);
		}
		return 0x01;
	}

	DWORD wirePlacementLessthan::usrExec() {
		return 0x01;
	}
	
}