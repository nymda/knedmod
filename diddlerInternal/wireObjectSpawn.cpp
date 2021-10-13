#include "wireObjectBase.h"
#include "wireObjectSpawn.h"
#include "wireObjectKernel.h"
#include "wirePlacements.h"
#include "objectSpawner.h"

namespace wireObjects {
	wireObj* spawnWireObject(wireObjectName object) {

		wireObjectInfo wobject = findByName(object);
		wireObj* newObject = 0;

		switch (wobject.name) {
			case wireObjectName::OBJ_IntBus:
				newObject = new wirePlacementIntBus();
				break;

			case wireObjectName::OBJ_BoolBus:
				newObject = new wirePlacementBoolBus();
				break;

			case wireObjectName::OBJ_ConstantValue:
				newObject = 0;
				break;

			case wireObjectName::OBJ_Explosive:
				newObject = new wirePlacementBomb();
				break;

			case wireObjectName::OBJ_GreaterThan:
				newObject = 0;
				break;

			case wireObjectName::OBJ_Lamp:
				newObject = new wirePlacementLamp();
				break;

			case wireObjectName::OBJ_LessThan:
				newObject = 0;
				break;

			case wireObjectName::OBJ_Raycast:
				newObject = 0;
				break;

			case wireObjectName::OBJ_Button:
				newObject = new wirePlacementButton();
				break;

			case wireObjectName::OBJ_BalloonDeployer:
				newObject = new wirePlacementBalloonDeployer();
				break;

			case wireObjectName::OBJ_ANDgate:
				newObject = new wirePlacementAND();
				break;

			case wireObjectName::OBJ_NOTgate:
				newObject = new wirePlacementNOT();
				break;

			case wireObjectName::OBJ_ORgate:
				newObject = new wirePlacementOR();
				break;
		}

		if (!newObject) { return 0; };

		spawner::childObjectSpawnParams params = {};
		params.useUserRotation = true;
		params.nocull = true;
		spawner::spawnedObject sp = spawner::placeChildObject(wobject.path, params);

		newObject->init(sp.shapes[0], 0);

		wireObjectStack.push_back(newObject);

		return newObject;
		
	}

	wireObjectInfo findByName(wireObjectName name) {
		for (wireObjectInfo& woi : validWireObjects) {
			if (woi.name == name) { return woi; };
		}
		return validWireObjects[0];
	}
}