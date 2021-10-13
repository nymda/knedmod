#pragma once
#include "wireObjectBase.h"
#include "wirePlacementGates.h"

namespace wireObjects {

	//AND
	wirePlacementAND::wirePlacementAND() {

	}

	DWORD wirePlacementAND::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.55f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Green, { 0.05f, 0.25f, 0.05f });
		this->nodes.push_back(_green);

		wireNode* _blue = new wireNode();
		_blue->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Blue, { 0.55f, 0.35f, 0.05f });
		this->nodes.push_back(_blue);

		return 0x01;
	}

	DWORD wirePlacementAND::exec() {
		if ((bool)this->nodes[0]->getValue() && (bool)this->nodes[2]->getValue()) {
			this->nodes[1]->setValue(1);
		}
		else {
			this->nodes[1]->setValue(0);
		}
		return 0x01;
	}

	DWORD wirePlacementAND::usrExec() {
		return 0x01;
	}

	//OR
	wirePlacementOR::wirePlacementOR() {

	}

	DWORD wirePlacementOR::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.55f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Green, { 0.05f, 0.25f, 0.05f });
		this->nodes.push_back(_green);

		wireNode* _blue = new wireNode();
		_blue->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Blue, { 0.55f, 0.35f, 0.05f });
		this->nodes.push_back(_blue);

		return 0x01;
	}

	DWORD wirePlacementOR::exec() {
		if ((bool)this->nodes[0]->getValue() || (bool)this->nodes[2]->getValue()) {
			this->nodes[1]->setValue(1);
		}
		else {
			this->nodes[1]->setValue(0);
		}
		return 0x01;
	}

	DWORD wirePlacementOR::usrExec() {
		return 0x01;
	}

	//NOT
	wirePlacementNOT::wirePlacementNOT() {

	}

	DWORD wirePlacementNOT::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.55f, 0.25f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Green, { 0.05f, 0.25f, 0.05f });
		this->nodes.push_back(_green);

		return 0x01;
	}

	DWORD wirePlacementNOT::exec() {
		if ((bool)this->nodes[0]->getValue()) {
			this->nodes[1]->setValue(0);
		}
		else {
			this->nodes[1]->setValue(1);
		}
		return 0x01;
	}

	DWORD wirePlacementNOT::usrExec() {
		return 0x01;
	}
}