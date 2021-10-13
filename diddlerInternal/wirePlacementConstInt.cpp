#include "wireObjectBase.h"
#include "wirePlacementConstInt.h"

namespace wireObjects {

	wirePlacementConstInt::wirePlacementConstInt() {

	}

	DWORD wirePlacementConstInt::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Red, { 0.25f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		return 0x01;
	}

	DWORD wirePlacementConstInt::exec() {
		nodes[0]->setValue(memory);
		return 0x01;
	}

	DWORD wirePlacementConstInt::usrExec() {
		return 0x01;
	}
	
}