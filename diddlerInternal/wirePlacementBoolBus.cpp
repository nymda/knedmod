#include "wireObjectBase.h"
#include "wirePlacementBoolBus.h"
#include "drawCube.h"

namespace wireObjects {
	wirePlacementBoolBus::wirePlacementBoolBus() {

	}

	DWORD wirePlacementBoolBus::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _yellow = new wireNode();
		_yellow->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Yellow, { 0.25f, 0.35f, 0.05f });
		this->nodes.push_back(_yellow);

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.05f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Green, { 0.05f, 0.35f, 0.05f });
		this->nodes.push_back(_green);

		wireNode* _blue = new wireNode();
		_blue->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Blue, { 0.05f, 0.55f, 0.05f });
		this->nodes.push_back(_blue);

		return 0x01;
	}

	DWORD wirePlacementBoolBus::exec() {

		int inputValue = nodes[0]->getValue(); //get the value on YELLOW
		nodes[1]->setValue(inputValue); //set YELLOW value on RED
		nodes[2]->setValue(inputValue); //set YELLOW value on GREEN
		nodes[3]->setValue(inputValue); //set YELLOW value on BLUE

		return 0x01;
	}

	DWORD wirePlacementBoolBus::usrExec()
	{
		return 0x01;
	}
}