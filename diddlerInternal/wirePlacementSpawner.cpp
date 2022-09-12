#include "wirePlacementSpawner.h"
#include "wireObjectBase.h"
#include "drawCube.h"
#include "tgtSpawner.h"
#include "toolgun.h"

namespace wireObjects {
	DWORD wirePlacementSpawnerDrawShapeOutline(TDVox* currentVox, glm::quat parentRotation, glm::vec3 spawnPosition) {
		td::Color boxColour = { 1.f, 1.f, 1.f, 1.f };

		glm::vec3 vx = parentRotation * glm::vec3(-1, 0, 0);
		glm::vec3 vy = parentRotation * glm::vec3(0, -1, 0);
		glm::vec3 vz = parentRotation * glm::vec3(0, 0, -1); //(UP)

		float voxSizeX = currentVox->sizeX / 10.f;
		float voxSizeY = currentVox->sizeY / 10.f;
		float voxSizeZ = currentVox->sizeZ / 10.f;

		//translations and localisations for the bounding box
		glm::vec3 translationFBL = ((vz * (voxSizeZ * 1.f)) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationBBR = ((vz * (voxSizeZ * 1.f)) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationBBL = ((vz * (voxSizeZ * 1.f)) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationFBR = ((vz * (voxSizeZ * 1.f)) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationFTL = ((vz * (voxSizeZ * 0.f)) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationBTR = ((vz * (voxSizeZ * 0.f)) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationBTL = ((vz * (voxSizeZ * 0.f)) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationFTR = ((vz * (voxSizeZ * 0.f)) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));

		td::Vec3 FBL = { spawnPosition.x - translationFBL.x, spawnPosition.y - translationFBL.y, spawnPosition.z - translationFBL.z };
		td::Vec3 BBR = { spawnPosition.x - translationBBR.x, spawnPosition.y - translationBBR.y, spawnPosition.z - translationBBR.z };
		td::Vec3 BBL = { spawnPosition.x - translationBBL.x, spawnPosition.y - translationBBL.y, spawnPosition.z - translationBBL.z };
		td::Vec3 FBR = { spawnPosition.x - translationFBR.x, spawnPosition.y - translationFBR.y, spawnPosition.z - translationFBR.z };

		td::Vec3 FTL = { spawnPosition.x - translationFTL.x, spawnPosition.y - translationFTL.y, spawnPosition.z - translationFTL.z };
		td::Vec3 BTR = { spawnPosition.x - translationBTR.x, spawnPosition.y - translationBTR.y, spawnPosition.z - translationBTR.z };
		td::Vec3 BTL = { spawnPosition.x - translationBTL.x, spawnPosition.y - translationBTL.y, spawnPosition.z - translationBTL.z };
		td::Vec3 FTR = { spawnPosition.x - translationFTR.x, spawnPosition.y - translationFTR.y, spawnPosition.z - translationFTR.z };

		//bottom square
		glb::oFDL(glb::renderer, FBL, FBR, boxColour, boxColour, false);
		glb::oFDL(glb::renderer, FBL, BBL, boxColour, boxColour, false);
		glb::oFDL(glb::renderer, BBL, BBR, boxColour, boxColour, false);
		glb::oFDL(glb::renderer, BBR, FBR, boxColour, boxColour, false);

		//top square
		glb::oFDL(glb::renderer, FTL, FTR, boxColour, boxColour, false);
		glb::oFDL(glb::renderer, FTL, BTL, boxColour, boxColour, false);
		glb::oFDL(glb::renderer, BTL, BTR, boxColour, boxColour, false);
		glb::oFDL(glb::renderer, BTR, FTR, boxColour, boxColour, false);

		//walls
		glb::oFDL(glb::renderer, FTL, FBL, boxColour, boxColour, false);
		glb::oFDL(glb::renderer, FTR, FBR, boxColour, boxColour, false);
		glb::oFDL(glb::renderer, BTL, BBL, boxColour, boxColour, false);
		glb::oFDL(glb::renderer, BTR, BBR, boxColour, boxColour, false);

		return 0x01;
	}

	wirePlacementSpawner::wirePlacementSpawner() {

	}

	DWORD wirePlacementSpawner::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;
		this->type = wireObjectName::OBJ_Spawner;

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Bool, wireObjects::nodeColour::Red, { 1.15f, 0.55f, 0.05f });
		this->nodes.push_back(_red);

		this->rcf.m_IgnoredShapes.push_back(this->housing);
		this->object = nToolgun::instance_spawner->currentSpawngunObject;
		//for (spawner::spawnerCatagory catig : spawner::spawnerObjectsDatabase) {
		//	for (spawner::LoadedSpawnableObject object : catig.objects) {
		//		if (object.objectName == "brick_metal") {
		//			this->object = object;
		//			break;
		//		}
		//	}
		//}

		return 0x01;
	}

	DWORD wirePlacementSpawner::exec() {
		td::Color red = { 1.f, 0.f, 0.f, 1.f };

		glm::quat targetShapeWorldrotation = math::expandRotation(math::q_td2glm(housing->getParentBody()->Rotation), math::q_td2glm(housing->rOffset));
		glm::vec3 targetShapeWorldPosition = math::expandPosition(math::q_td2glm(housing->getParentBody()->Rotation), math::v3_td2glm(housing->getParentBody()->Position), math::v3_td2glm(housing->pOffset));

		glm::vec3 vx = targetShapeWorldrotation * glm::vec3(1, 0, 0);
		glm::vec3 vy = targetShapeWorldrotation * glm::vec3(0, 1, 0);
		glm::vec3 vz = targetShapeWorldrotation * glm::vec3(0, 0, 1); //(UP)

		float voxSizeX_p = ((float)housing->pVox->sizeX / 10.f) / 2.f;
		float voxSizeY_p = ((float)housing->pVox->sizeY / 10.f) / 2.f;
		float voxSizeZ_p = ((float)housing->pVox->sizeZ / 10.f) / 2.f;
		glm::vec3 centerpoint = targetShapeWorldPosition + ((vz * voxSizeZ_p) + (vy * voxSizeY_p) + (vx * voxSizeX_p));
		glm::vec3 extendpoint = targetShapeWorldPosition + ((vz * 2.f) + (vy * voxSizeY_p) + (vx * voxSizeX_p));
		glm::vec3 spawnpoint = targetShapeWorldPosition + ((vz * 0.4f) + (vy * 0.55f) + (vx * 0.55f));

		//drawCube(math::v3_glm2td(spawnpoint), 0.05f, red);

		glm::vec3 hitDir = glm::normalize(vz);
		glm::quat q = glm::conjugate(glm::quat(glm::lookAt(centerpoint, centerpoint + -hitDir, vx)));

		wirePlacementSpawnerDrawShapeOutline(this->object.voxObject, q, spawnpoint);

		if ((this->previousInputState != (bool)(nodes[0]->getValue())) && (bool)(nodes[0]->getValue())) {

			spawner::freeObjectSpawnParams fosp = {};
			spawner::spawnedObject object = {};
			fosp.attributes = this->object.attributes;
			spawnFreeEntity(this->object.voxPath, fosp, &object);

			glm::vec3 vx_q = q * glm::vec3(1, 0, 0);
			glm::vec3 vy_q = q * glm::vec3(0, 1, 0);
			glm::vec3 vz_q = q * glm::vec3(0, 0, 1); //(UP)
			float sVoxSizeX = this->object.voxObject->sizeX / 10.f;
			float sVoxSizeY = this->object.voxObject->sizeY / 10.f;
			float sVoxSizeZ = this->object.voxObject->sizeZ / 10.f;
			glm::vec3 spawnTranslation = ((vz_q * (0.f)) + (vy_q * (sVoxSizeY / 2.f)) + (vx_q * (sVoxSizeX / 2.f)));

			object.body->Position = { spawnpoint.x - spawnTranslation.x, spawnpoint.y - spawnTranslation.y, spawnpoint.z - spawnTranslation.z };

			glm::vec3 newVelocity = vz_q * (float)this->memory;
			object.body->Velocity = { newVelocity.x, newVelocity.y, newVelocity.z };
			*(glm::quat*)&object.body->Rotation = q;

			for (TDShape* cShape : object.shapes) {
				for (spawner::objectAttribute att : fosp.attributes) {
					glb::oSOA(cShape, &att.attribute, &att.level);
				}
			}

		}
		this->previousInputState = (bool)(nodes[0]->getValue());
		return 0x01;
	}

	DWORD wirePlacementSpawner::usrExec() {
		this->object = nToolgun::instance_spawner->currentSpawngunObject;
		return 0x01;
	}
}