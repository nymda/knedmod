#include "Raycaster.h"
#include "wireObjectKernel.h"
#include "imgui/imgui.h"
#include "wireTool.h"
#include "wireObjectSpawn.h"

namespace wireObjects {
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

									printf_s("Connecting nodes: %p <-> %p", nodeA, nodeB);
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

			if (glb::player->isAttacking) {
				if (shootOnce) {
					shootOnce = false;

					wireObjects::wireObj* hitObj = 0;
					raycaster::rayData rd = raycaster::castRayPlayer();
					if (wireObjects::getWireObjectByShape(rd.hitShape, &hitObj) && rd.distance <= 3.f) {
						hitObj->memory = targetUserMemory;
					}
				}
			}
			else {
				shootOnce = true;
			}
		}
	}
}