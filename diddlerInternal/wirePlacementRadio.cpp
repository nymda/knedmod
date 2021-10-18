#include "wireObjectBase.h"
#include "wirePlacementRadio.h"

namespace wireObjects {
	wirePlacementRadioTx::wirePlacementRadioTx() {

	}

	DWORD wirePlacementRadioTx::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _blue = new wireNode();
		_blue->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Blue, { 0.55f, 0.05f, 0.05f });
		this->nodes.push_back(_blue);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_WirelessBool, wireObjects::nodeColour::Blue, { 0.05f, 0.15f, 0.05f });
		this->nodes.push_back(_green);

		return 0x01;
	}

	DWORD wirePlacementRadioTx::exec() {

		int antennaVal = nodes[0]->getValue();
		nodes[1]->setValue(antennaVal);

		if (nodes[1]->isConnected()) {
			this->housing->Intergrity = 1.f;
		}
		else {
			this->housing->Intergrity = 0.f;
		}

		return 0x01;
	}

	DWORD wirePlacementRadioTx::usrExec() {
		return 0x01;
	}

	///--------------------------------------------------------------------------

	wirePlacementRadioRx::wirePlacementRadioRx() {

	}

	DWORD wirePlacementRadioRx::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.55f, 0.05f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_WirelessBool, wireObjects::nodeColour::Green, { 0.05f, 0.15f, 0.05f });
		this->nodes.push_back(_green);

		return 0x01;
	}

	DWORD wirePlacementRadioRx::exec() {

		int antennaVal = nodes[1]->getValue();
		nodes[0]->setValue(antennaVal);

		if (nodes[1]->isConnected()) {
			this->housing->Intergrity = 1.f;
		}
		else {
			this->housing->Intergrity = 0.f;
		}

		return 0x01;
	}

	DWORD wirePlacementRadioRx::usrExec() {
		return 0x01;
	}

}