#include "wireObjectBase.h"
#include "Raycaster.h"
#include <vector>
#include "objectSpawner.h"
#include "wireObjectSpawn.h"

namespace wireObjects {
	std::vector<wireObj*> wireObjectStack = {};

	bool getWireObjectByShape(TDShape* shape, wireObj** out) {
		for (wireObj* wObj : wireObjectStack) {
			if (wObj->housing == shape) {
				*out = wObj;
				return true;
			}
		}
		return false;
	}

	void highlightTarget() {
		raycaster::rayData rd = raycaster::castRayPlayer();
		if (rd.successful) {
			wireNode* tNode = 0;
			wireObj* tObj = 0;
			if (getWireObjectByShape(rd.hitShape, &tObj)) {
				if(tObj->getClosestNode(math::v3_td2glm(rd.worldPos), &tNode)) {
					tObj->drawNodes(tNode);
				}
				else {
					tObj->drawNodes(0);
				}
			}
		}
	}

	int loadWireObjectVoxs() {
		int r = 0;
		for (wireObjectInfo& woi : validWireObjects) {
			td::small_string ssVoxPath = td::small_string(woi.path);
			td::small_string ssSubPath = td::small_string("");
			woi.voxTemplate = (TDVox*)glb::oSpawnVox(&ssVoxPath, &ssSubPath, 1.f);

			if (woi.voxTemplate) {
				printf_s("Loaded vox template for %s\n", woi.path);
				r++;
			}
			else {
				printf_s("Vox tempalte loading failed for %s\n", woi.path);
			}
		}
		return r;
	}

	int updateExistingWireObjects() {
		if (glb::game->m_LoadingEffect != 0 && wireObjectStack.size() > 0) {
			for (wireObj* wObj : wireObjectStack) {
				wObj->dispose();
			}

			wireObjectStack.clear();
			return -1;
		}

		int c = 0;
		for (wireObj* wObj : wireObjectStack) {
			if (!wObj->housing || wObj->housing->isBroken) { wObj->softDispose(); continue; }
			if (!wObj->destroyed) {
				wObj->exec();
				c++; //haha
			}
		}

		return c;
	}
}