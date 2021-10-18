#include "wireObjectBase.h"
#include "wireObjectSpawn.h"
#include "wireObjectKernel.h"
#include "wirePlacements.h"
#include "objectSpawner.h"

namespace wireObjects {
    float objectPlacementRotationSteps_H[] = { 0.f, 45.f, 90.f, 135.f, 180.f, 225.f, 270.f, 315.f };
    float objectPlacementRotationSteps_V[] = { 0.f, 90.f, 180.f, 270.f };
    int currentRotationStep_H = 0;

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
				newObject = new wirePlacementConstInt();
				break;

			case wireObjectName::OBJ_Explosive:
				newObject = new wirePlacementBomb();
				break;

			case wireObjectName::OBJ_GreaterThan:
				newObject = new wirePlacementGreaterthan();
				break;

			case wireObjectName::OBJ_Lamp:
				newObject = new wirePlacementLamp();
				break;

			case wireObjectName::OBJ_LessThan:
				newObject = new wirePlacementLessthan();
				break;

			case wireObjectName::OBJ_Raycast:
                newObject = new wirePlacementRaycaster();
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

			case wireObjectName::OBJ_PositionTracker:
				newObject = new wirePlacementGPS();
				break;

            case wireObjectName::OBJ_RadioTx:
                newObject = new wirePlacementRadioTx();
                break;

            case wireObjectName::OBJ_RadioRx:
                newObject = new wirePlacementRadioRx();
                break;
		}

		if (!newObject) { return 0; };

		spawner::childObjectSpawnParams params = {};
		params.useUserRotation = false;
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



    //everything below here is the drawing stuff copied from objectSpawner.cpp. i cant be bothered to redo the maths for that shit. 


    float deg2rad(float deg) {
        return (deg * math::pi) / 180;
    }

    td::Vec3 getClippingTranslation(TDVox* currentVox, raycaster::rayData rd, bool useUserRotation) {
        td::Vec3 target = rd.worldPos;

        td::Color boxColour = { 0.f, 1.f, 0.f, 1.f };

        float voxSizeX = (currentVox->sizeX / 10.f);
        float voxSizeY = (currentVox->sizeY / 10.f);
        float voxSizeZ = (currentVox->sizeZ / 10.f);

        td::Vec3 oSize = { voxSizeX, voxSizeY, voxSizeZ };
        glm::vec3 hitPos = { rd.worldPos.x, rd.worldPos.y, rd.worldPos.z };

        glm::quat facePlayer = glm::quat(glm::vec3(4.71238898025f, glb::player->camYaw /* + (deg2rad(objectPlacementRotationSteps[currentRotationStep]))*/, 0));
        glm::vec3 vxTmp = facePlayer * glm::vec3(-1, 0, 0);

        glm::vec3 hitDir = glm::vec3(rd.angle.x, rd.angle.y, rd.angle.z);

        hitDir = glm::normalize(hitDir);

        glm::quat q = glm::conjugate(glm::quat(glm::lookAt(hitPos, hitPos + hitDir, vxTmp))); //this is kinda inverted, with "up" facing the player and "forward" facing away from the surface. "fixing" this makes it work less good so eh.
        glm::quat q_flat = q;

        if (useUserRotation) {
            glm::quat rotOffset = glm::quat(glm::vec3(0.f, 0.f, -(deg2rad(objectPlacementRotationSteps_H[currentRotationStep_H]))));
            q = q * rotOffset;
        }

        glm::vec3 vx = q * glm::vec3(-1, 0, 0);
        glm::vec3 vy = q * glm::vec3(0, -1, 0);
        glm::vec3 vz = q * glm::vec3(0, 0, -1); //(UP)

        glm::vec3 translationFBL = ((vz * (voxSizeZ * 0.5f)) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
        glm::vec3 translationBBR = ((vz * (voxSizeZ * 0.5f)) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
        glm::vec3 translationBBL = ((vz * (voxSizeZ * 0.5f)) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
        glm::vec3 translationFBR = ((vz * (voxSizeZ * 0.5f)) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));

        glm::vec3 translationFTL = ((vz * (voxSizeZ * -0.5f)) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
        glm::vec3 translationBTR = ((vz * (voxSizeZ * -0.5f)) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
        glm::vec3 translationBTL = ((vz * (voxSizeZ * -0.5f)) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
        glm::vec3 translationFTR = ((vz * (voxSizeZ * -0.5f)) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));

        td::Vec3 FBL = { target.x - translationFBL.x, target.y - translationFBL.y, target.z - translationFBL.z };
        glm::vec3 FBL_Local = math::localisePosition(q_flat, math::v3_td2glm(rd.worldPos), math::v3_td2glm(FBL));

        td::Vec3 BBR = { target.x - translationBBR.x, target.y - translationBBR.y, target.z - translationBBR.z };
        glm::vec3 BBR_Local = math::localisePosition(q_flat, math::v3_td2glm(rd.worldPos), math::v3_td2glm(BBR));

        td::Vec3 BBL = { target.x - translationBBL.x, target.y - translationBBL.y, target.z - translationBBL.z };
        glm::vec3 BBL_Local = math::localisePosition(q_flat, math::v3_td2glm(rd.worldPos), math::v3_td2glm(BBL));

        td::Vec3 FBR = { target.x - translationFBR.x, target.y - translationFBR.y, target.z - translationFBR.z };
        glm::vec3 FBR_Local = math::localisePosition(q_flat, math::v3_td2glm(rd.worldPos), math::v3_td2glm(FBR));

        td::Vec3 FTL = { target.x - translationFTL.x, target.y - translationFTL.y, target.z - translationFTL.z };
        glm::vec3 FTL_Local = math::localisePosition(q_flat, math::v3_td2glm(rd.worldPos), math::v3_td2glm(FTL));

        td::Vec3 BTR = { target.x - translationBTR.x, target.y - translationBTR.y, target.z - translationBTR.z };
        glm::vec3 BTR_Local = math::localisePosition(q_flat, math::v3_td2glm(rd.worldPos), math::v3_td2glm(BTR));

        td::Vec3 BTL = { target.x - translationBTL.x, target.y - translationBTL.y, target.z - translationBTL.z };
        glm::vec3 BTL_Local = math::localisePosition(q_flat, math::v3_td2glm(rd.worldPos), math::v3_td2glm(BTL));

        td::Vec3 FTR = { target.x - translationFTR.x, target.y - translationFTR.y, target.z - translationFTR.z };
        glm::vec3 FTR_Local = math::localisePosition(q_flat, math::v3_td2glm(rd.worldPos), math::v3_td2glm(FTR));

        td::Vec3 boundaryPositions[8] = { FBL, BBR, BBL, FBR, FTL, BTR, BTL, FTR };
        glm::vec3 localisedBoundaryPositions[8] = { FBL_Local, BBR_Local, BBL_Local, FBR_Local, FTL_Local, BTR_Local, BTL_Local, FTR_Local };

        float floorHeight = rd.worldPos.y;
        bool floorClipping = false;

        float ZOffset = 0.f;

        int _ptr = 0;
        for (glm::vec3 current : localisedBoundaryPositions) {
            if (current.z < 0.f) {
                //point is not clipping
            }
            else {
                //point is clipping
                if (current.z > ZOffset) {
                    ZOffset = current.z;
                }
            }
            _ptr++;
        }
        return { 0.f, 0.f, ZOffset };
    }

    td::Color red{ 1.f, 0.f, 0.f, 1.f };
    void drawWireObjectOutline(wireObjectInfo object, raycaster::rayData rd) {

        glm::vec3 clippingOffset = math::v3_td2glm(getClippingTranslation(object.voxTemplate, rd, true));


        td::Vec3 target = rd.worldPos;

        td::Color boxColour = { 1.f, 1.f, 1.f, 1.f };

        float voxSizeX = (object.voxTemplate->sizeX / 10.f);
        float voxSizeY = (object.voxTemplate->sizeY / 10.f);
        float voxSizeZ = (object.voxTemplate->sizeZ / 10.f);

        td::Vec3 oSize = { voxSizeX, voxSizeY, voxSizeZ };
        glm::vec3 targetGLM = math::v3_td2glm(target);

        glm::quat facePlayer = glm::quat(glm::vec3(4.71238898025f, glb::player->camYaw /* + (deg2rad(objectPlacementRotationSteps[currentRotationStep]))*/, 0));
        glm::vec3 vxTmp = facePlayer * glm::vec3(-1, 0, 0);

        glm::vec3 hitDir = glm::vec3(rd.angle.x, rd.angle.y, rd.angle.z);

        hitDir = glm::normalize(hitDir);

        glm::quat q = glm::conjugate(glm::quat(glm::lookAt(targetGLM, targetGLM + hitDir, vxTmp))); //this is kinda inverted, with "up" facing the player and "forward" facing away from the surface. "fixing" this makes it work less good so eh.
        glm::quat q_flat = q;

        glm::quat rotOffset = glm::quat(glm::vec3(0.f, 0.f, -(deg2rad(objectPlacementRotationSteps_H[currentRotationStep_H]))));
        q = q * rotOffset;

        glm::vec3 vx = q * glm::vec3(-1, 0, 0);
        glm::vec3 vy = q * glm::vec3(0, -1, 0);
        glm::vec3 vz = q * glm::vec3(0, 0, -1); //(UP)

        glm::vec3 vx_f = q_flat * glm::vec3(1, 0, 0);
        glm::vec3 vy_f = q_flat * glm::vec3(0, 1, 0);
        glm::vec3 vz_f = q_flat * glm::vec3(0, 0, 1); //(UP)

        glm::vec3 clippingTranslation = (vz_f * clippingOffset.z) + (vy_f * clippingOffset.y) + (vx_f * clippingOffset.x);

        //translations and localisations for the bounding box
        glm::vec3 translationFBL = ((vz * (voxSizeZ * 0.5f)) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
        translationFBL += clippingTranslation;

        glm::vec3 translationBBR = ((vz * (voxSizeZ * 0.5f)) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
        translationBBR += clippingTranslation;

        glm::vec3 translationBBL = ((vz * (voxSizeZ * 0.5f)) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
        translationBBL += clippingTranslation;

        glm::vec3 translationFBR = ((vz * (voxSizeZ * 0.5f)) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
        translationFBR += clippingTranslation;

        glm::vec3 translationFTL = ((vz * (voxSizeZ * -0.5f)) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
        translationFTL += clippingTranslation;

        glm::vec3 translationBTR = ((vz * (voxSizeZ * -0.5f)) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
        translationBTR += clippingTranslation;

        glm::vec3 translationBTL = ((vz * (voxSizeZ * -0.5f)) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
        translationBTL += clippingTranslation;

        glm::vec3 translationFTR = ((vz * (voxSizeZ * -0.5f)) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
        translationFTR += clippingTranslation;

        //translations and localisations for the indicator boxes

        //red front box
        glm::vec3 translationFTRI_IF = ((vz * (voxSizeZ * 0.4f)) + (vy * (voxSizeY * 0.4f)) - (vx * (voxSizeX * 0.5f)));
        translationFTRI_IF += clippingTranslation;

        glm::vec3 translationFBRI_IF = ((vz * (voxSizeZ * -0.4f)) + (vy * (voxSizeY * 0.4f)) - (vx * (voxSizeX * 0.5f)));
        translationFBRI_IF += clippingTranslation;

        glm::vec3 translationBTRI_IF = ((vz * (voxSizeZ * 0.4f)) - (vy * (voxSizeY * 0.4f)) - (vx * (voxSizeX * 0.5f)));
        translationBTRI_IF += clippingTranslation;

        glm::vec3 translationBBRI_IF = ((vz * (voxSizeZ * -0.4f)) - (vy * (voxSizeY * 0.4f)) - (vx * (voxSizeX * 0.5f)));
        translationBBRI_IF += clippingTranslation;

        //blue top box
        glm::vec3 translationFTRI_IT = ((vz * (voxSizeZ * -0.5f)) + (vy * (voxSizeY * 0.3f)) - (vx * (voxSizeX * 0.3f)));
        translationFTRI_IT += clippingTranslation;

        glm::vec3 translationBTRI_IT = ((vz * (voxSizeZ * -0.5f)) - (vy * (voxSizeY * 0.3f)) - (vx * (voxSizeX * 0.3f)));
        translationBTRI_IT += clippingTranslation;

        glm::vec3 translationFTLI_IT = ((vz * (voxSizeZ * -0.5f)) + (vy * (voxSizeY * 0.3f)) + (vx * (voxSizeX * 0.3f)));
        translationFTLI_IT += clippingTranslation;

        glm::vec3 translationBTLI_IT = ((vz * (voxSizeZ * -0.5f)) - (vy * (voxSizeY * 0.3f)) + (vx * (voxSizeX * 0.3f)));
        translationBTLI_IT += clippingTranslation;

        td::Vec3 FBL = { target.x - translationFBL.x, target.y - translationFBL.y, target.z - translationFBL.z };
        td::Vec3 BBR = { target.x - translationBBR.x, target.y - translationBBR.y, target.z - translationBBR.z };
        td::Vec3 BBL = { target.x - translationBBL.x, target.y - translationBBL.y, target.z - translationBBL.z };
        td::Vec3 FBR = { target.x - translationFBR.x, target.y - translationFBR.y, target.z - translationFBR.z };

        td::Vec3 FTL = { target.x - translationFTL.x, target.y - translationFTL.y, target.z - translationFTL.z };
        td::Vec3 BTR = { target.x - translationBTR.x, target.y - translationBTR.y, target.z - translationBTR.z };
        td::Vec3 BTL = { target.x - translationBTL.x, target.y - translationBTL.y, target.z - translationBTL.z };
        td::Vec3 FTR = { target.x - translationFTR.x, target.y - translationFTR.y, target.z - translationFTR.z };

        td::Vec3 FTRI_IF = { target.x - translationFTRI_IF.x, target.y - translationFTRI_IF.y, target.z - translationFTRI_IF.z };
        td::Vec3 FBRI_IF = { target.x - translationFBRI_IF.x, target.y - translationFBRI_IF.y, target.z - translationFBRI_IF.z };
        td::Vec3 BTRI_IF = { target.x - translationBTRI_IF.x, target.y - translationBTRI_IF.y, target.z - translationBTRI_IF.z };
        td::Vec3 BBRI_IF = { target.x - translationBBRI_IF.x, target.y - translationBBRI_IF.y, target.z - translationBBRI_IF.z };

        td::Vec3 FTRI_IT = { target.x - translationFTRI_IT.x, target.y - translationFTRI_IT.y, target.z - translationFTRI_IT.z };
        td::Vec3 BTRI_IT = { target.x - translationBTRI_IT.x, target.y - translationBTRI_IT.y, target.z - translationBTRI_IT.z };
        td::Vec3 FTLI_IT = { target.x - translationFTLI_IT.x, target.y - translationFTLI_IT.y, target.z - translationFTLI_IT.z };
        td::Vec3 BTLI_IT = { target.x - translationBTLI_IT.x, target.y - translationBTLI_IT.y, target.z - translationBTLI_IT.z };
        //td::Vec3 frontcenter = { target.x - translationCEN.x, target.y - translationCEN.y, target.z - translationCEN.z };

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

        //front indicator
        glb::oFDL(glb::renderer, FTRI_IF, BTRI_IF, red, red, false);
        glb::oFDL(glb::renderer, FBRI_IF, BBRI_IF, red, red, false);
        glb::oFDL(glb::renderer, FBRI_IF, FTRI_IF, red, red, false);
        glb::oFDL(glb::renderer, BBRI_IF, BTRI_IF, red, red, false);
    }
}