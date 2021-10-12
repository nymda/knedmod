#include "wireObjectBase.h"
#include "wirePlacementBalloonDeployer.h"
#include "objectSpawner.h"
#include "balloons.h"

namespace wireObjects {

	wirePlacementBalloonDeployer::wirePlacementBalloonDeployer() {

	}

	DWORD wirePlacementBalloonDeployer::init(TDShape* housing, int memoryVal) {

		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 0.45f, 0.25f, 0.05f });
		this->nodes.push_back(_red);

		return 0x01;
	}

	int cooldown = 0;
	DWORD wirePlacementBalloonDeployer::exec() {

		if ((bool)this->nodes[0]->getValue() && cooldown == 0) {
			cooldown = 30;
			spawner::freeObjectSpawnParams fosp = {};

			td::Vec3 objectMin = this->housing->posMin;
			td::Vec3 objectMax = this->housing->posMax;
			td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };

			fosp.useSnaps = true;
			fosp.snapPosition = math::v3_td2glm(centerpoint);
			spawner::spawnedObject obj = spawner::placeFreeObject("KM_Vox\\Default\\balloon\\object.vox", fosp);
			obj.body->Position.y += 1.f;

			raycaster::rayData rd = raycaster::castRayPlayer();

			balloons::balloon b = { obj.shapes[0], obj.body, 25.f, false, false, 2, centerpoint };
			balloons::balloons.push_back(b);
		}
		else {
			if (cooldown > 0) {
				cooldown--;
			}
		}

		return 0x01;
	}

	DWORD wirePlacementBalloonDeployer::usrExec() {

		return 0x01;
	}
}