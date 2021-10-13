#include "wireObjectBase.h"
#include "wirePlacementGPS.h"

namespace wireObjects {

	wirePlacementGPS::wirePlacementGPS() {

	}

	DWORD wirePlacementGPS::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _blue = new wireNode();
		_blue->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Blue, { 0.45f, 0.15f, 0.05f });
		this->nodes.push_back(_blue);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Green, { 0.45f, 0.35f, 0.05f });
		this->nodes.push_back(_green);

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Red, { 0.45f, 0.55f, 0.05f });
		this->nodes.push_back(_red);

		return 0x01;
	}

	DWORD wirePlacementGPS::exec() {
		td::Vec3 objectMin = this->housing->posMin;
		td::Vec3 objectMax = this->housing->posMax;
		td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };

		nodes[0]->setValue(convFloatToInt(centerpoint.x));
		nodes[1]->setValue(convFloatToInt(centerpoint.y));
		nodes[2]->setValue(convFloatToInt(centerpoint.z));

		return 0x01;
	}

	DWORD wirePlacementGPS::usrExec() {
		return 0x01;
	}
	
}