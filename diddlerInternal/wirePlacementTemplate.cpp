#include "wireObjectBase.h"
#include "wirePlacementTemplate.h"

namespace wireObjects {
	wirePlacementTemplate::wirePlacementTemplate() {

	}

	DWORD wirePlacementTemplate::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.45f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		return 0x01;
	}

	DWORD wirePlacementTemplate::exec() {


		return 0x01;
	}

	DWORD wirePlacementTemplate::usrExec() {
		return 0x01;
	}

}