#include "Raycaster.h"
#include "wireObjectKernel.h"
#include "imgui/imgui.h"
#include "wireTool.h"
#include "wireObjectSpawn.h"

namespace wireObjects {
    td::Vec3 getWireClip(TDVox* currentVox, raycaster::rayData rd);
    void projectNewWireObj(TDVox* currentVox);

    ImGuiWindowFlags infoBoxFlags;
	wireToolSetting toolgunSetting = wireToolSetting::WTS_Interact;
	wireObjectName toolgunSelectedObject = wireObjectName::OBJ_Button;

	bool shootOnce = true;
	int targetUserMemory = 0;

	wireObj* tObj = 0;
	wireNode* nodeA = 0;
	wireNode* nodeB = 0;
	int stage = 0;

	void execTool() {
		if (toolgunSetting == wireToolSetting::WTS_Interact) {
			raycaster::rayData rd = raycaster::castRayPlayer();
			if (rd.successful) {
				wireNode* tNode = 0;
				if (getWireObjectByShape(rd.hitShape, &tObj)) {
					if (tObj->getClosestNode(math::v3_td2glm(rd.worldPos), &tNode)) {
						tObj->drawNodes(tNode);

						if (glb::player->isAttacking) {
							if (shootOnce) {
								if (stage == 0) {
									stage = 1;
									shootOnce = false;
									nodeA = tNode;
								}
								else if (stage == 1) {
									stage = 0;
									shootOnce = false;
									nodeB = tNode;

									printf_s("Connecting nodes: %p <-> %p\n", nodeA, nodeB);
									connectNodes(nodeA, nodeB);
									nodeA = 0;
									nodeB = 0;
								}
							}
						}
						else {
							shootOnce = true;
						}

					}
                    else {
                        tObj->drawNodes(0);
                    }
				}
				else {
					tObj = 0;
				}
			}

			if (nodeA) {
				td::Vec3 nodeApos = math::v3_glm2td(nodeA->getWorldPosition());
				td::Vec3 nodeBpos = rd.worldPos;
				glb::oFDL(glb::renderer, nodeApos, nodeBpos, nodeColourActive[(int)nodeA->getColour()], nodeColourActive[(int)nodeA->getColour()], false);
			}

			if (tObj) {
				infoBoxFlags = 0;
				infoBoxFlags |= ImGuiWindowFlags_NoMove;
				infoBoxFlags |= ImGuiWindowFlags_NoCollapse;
				infoBoxFlags |= ImGuiWindowFlags_NoTitleBar;
				infoBoxFlags |= ImGuiWindowFlags_AlwaysAutoResize;

				ImGui::SetNextWindowPos({ (float)((glb::game->m_ResX / 2)), (float)((glb::game->m_ResY / 2)) });
				ImGui::Begin("WireObjInfo", (bool*)false, infoBoxFlags);
				ImGui::Text("Object: %p", tObj);
				ImGui::Text("Memory: %i", tObj->memory);

				for (wireNode* cNode : tObj->nodes) {
					if (cNode->getType() == wireObjects::nodeType::NT_Bool) {
						ImGui::Text("Node: %s | Connected: %s | Value: %s", nodeColourNames[(int)cNode->getColour()], (cNode->isConnected() ? "True" : "False"), ((bool)cNode->getOwnValue() ? "True" : "False"));
					}
					else if (cNode->getType() == wireObjects::nodeType::NT_Numeric) {
						ImGui::Text("Node: %s | Connected: %s | Value: %i", nodeColourNames[(int)cNode->getColour()], (cNode->isConnected() ? "True" : "False"), cNode->getOwnValue());
					}
				}

				ImGui::End();
			}
		}
		else if (toolgunSetting == wireToolSetting::WTS_Place) {

			if (nodeA != 0) { nodeA = 0; }
			if (nodeB != 0) { nodeB = 0; }

            TDVox* cVoxTmplate = findByName(toolgunSelectedObject).voxTemplate;
            if (cVoxTmplate) {
                projectNewWireObj(cVoxTmplate);
            }

			if (glb::player->isAttacking) {
				if (shootOnce) {
					shootOnce = false;
					wireObjects::spawnWireObject(toolgunSelectedObject);
				}
			}
			else {
				shootOnce = true;
			}
		}
		else if (toolgunSetting == wireToolSetting::WTS_SetUserMemory) {
			if (nodeA != 0) { nodeA = 0; }
			if (nodeB != 0) { nodeB = 0; }

            raycaster::rayData rd = raycaster::castRayPlayer();
            bool isWireObj = wireObjects::getWireObjectByShape(rd.hitShape, &tObj);

            if (isWireObj) {
                infoBoxFlags = 0;
                infoBoxFlags |= ImGuiWindowFlags_NoMove;
                infoBoxFlags |= ImGuiWindowFlags_NoCollapse;
                infoBoxFlags |= ImGuiWindowFlags_NoTitleBar;
                infoBoxFlags |= ImGuiWindowFlags_AlwaysAutoResize;

                ImGui::SetNextWindowPos({ (float)((glb::game->m_ResX / 2)), (float)((glb::game->m_ResY / 2)) });
                ImGui::Begin("WireObjInfo", (bool*)false, infoBoxFlags);
                ImGui::Text("Object: %p", tObj);
                ImGui::Text("Memory: %i", tObj->memory);
                ImGui::End();
            }

			if (glb::player->isAttacking && isWireObj) {
				if (shootOnce) {
					shootOnce = false;
					if (rd.distance <= 3.f) {
                        td::Color white = { 1.f, 1.f, 1.f, 1.f };
                        glb::oOutlineShape(glb::renderer, rd.hitShape, &white, 1.f);
                        tObj->memory = targetUserMemory;
					}
				}
			}
			else {
				shootOnce = true;
			}
		}
	}

    td::Vec3 getWireClip(TDVox* currentVox, raycaster::rayData rd) {
        td::Vec3 target = rd.worldPos;

        td::Color boxColour = { 0.f, 1.f, 0.f, 1.f };

        float voxSizeX = (currentVox->sizeX / 10.f) * spawner::voxScale;
        float voxSizeY = (currentVox->sizeY / 10.f) * spawner::voxScale;
        float voxSizeZ = (currentVox->sizeZ / 10.f) * spawner::voxScale;

        td::Vec3 oSize = { voxSizeX, voxSizeY, voxSizeZ };
        glm::vec3 hitPos = { rd.worldPos.x, rd.worldPos.y, rd.worldPos.z };

        glm::quat facePlayer = glm::quat(glm::vec3(4.71238898025f, glb::player->camYaw /* + (deg2rad(objectPlacementRotationSteps[currentRotationStep]))*/, 0));
        glm::vec3 vxTmp = facePlayer * glm::vec3(-1, 0, 0);

        glm::vec3 hitDir = glm::vec3(rd.angle.x, rd.angle.y, rd.angle.z);

        hitDir = glm::normalize(hitDir);

        glm::quat q = glm::conjugate(glm::quat(glm::lookAt(hitPos, hitPos + hitDir, vxTmp))); //this is kinda inverted, with "up" facing the player and "forward" facing away from the surface. "fixing" this makes it work less good so eh.
        glm::quat q_flat = q;

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
            if (!(current.z < 0.f)) {
                if (current.z > ZOffset) {
                    ZOffset = current.z;
                }
            }
            _ptr++;
        }
        return { 0.f, 0.f, ZOffset };
    }

    void projectNewWireObj(TDVox* currentVox) {

        raycaster::rayData rd = raycaster::castRayPlayer();

        glm::vec3 clippingOffset = math::v3_td2glm(getWireClip(currentVox, rd));

        td::Vec3 target = rd.worldPos;

        td::Color boxColour = { 1.f, 1.f, 1.f, 1.f };

        float voxSizeX = (currentVox->sizeX / 10.f) * spawner::voxScale;
        float voxSizeY = (currentVox->sizeY / 10.f) * spawner::voxScale;
        float voxSizeZ = (currentVox->sizeZ / 10.f) * spawner::voxScale;

        td::Vec3 oSize = { voxSizeX, voxSizeY, voxSizeZ };
        glm::vec3 targetGLM = math::v3_td2glm(target);

        glm::quat facePlayer = glm::quat(glm::vec3(4.71238898025f, glb::player->camYaw /* + (deg2rad(objectPlacementRotationSteps[currentRotationStep]))*/, 0));
        glm::vec3 vxTmp = facePlayer * glm::vec3(-1, 0, 0);

        glm::vec3 hitDir = glm::vec3(rd.angle.x, rd.angle.y, rd.angle.z);

        hitDir = glm::normalize(hitDir);

        glm::quat q = glm::conjugate(glm::quat(glm::lookAt(targetGLM, targetGLM + hitDir, vxTmp))); //this is kinda inverted, with "up" facing the player and "forward" facing away from the surface. "fixing" this makes it work less good so eh.
        glm::quat q_flat = q;

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

    }
}