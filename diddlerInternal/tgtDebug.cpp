#include "tgtDebug.h"
#include "Raycaster.h"
#include "types.h"
#include "imgui/imgui.h"

tgt_debug::tgt_debug() {

}

DWORD tgt_debug::exec() {
	td::Color green{ 0.f, 1.f, 0.f, 1.f };
	td::Color blue{ 0.f, 0.f, 1.f, 1.f };

	raycaster::rayData rayDat = raycaster::castRayPlayer();
	if (rayDat.hitShape) {
		infoBoxFlags = 0;
		infoBoxFlags |= ImGuiWindowFlags_NoMove;
		infoBoxFlags |= ImGuiWindowFlags_NoCollapse;
		infoBoxFlags |= ImGuiWindowFlags_NoTitleBar;
		infoBoxFlags |= ImGuiWindowFlags_AlwaysAutoResize;

		glb::oOutlineShape(glb::renderer, rayDat.hitShape, &green, 1.f);

		ImGui::SetNextWindowPos({ (float)((glb::game->m_ResX / 2)), (float)((glb::game->m_ResY / 2)) });
		ImGui::Begin("debugInfo", (bool*)false, infoBoxFlags);
		TDShape* cshape = rayDat.hitShape;
		TDBody* cbody = rayDat.hitShape->getParentBody();

		ImGui::Text("Shape: 0x%p (0x%p)", cshape, (TDShape*)((uintptr_t)cshape + (uintptr_t)0x28));
		Entity* currentShapeSibling = cshape->pSibling;
		int shapeSiblingCount = 0;
		while (currentShapeSibling != 0x00 && shapeSiblingCount<10) {
			shapeSiblingCount++;
			ImGui::Text("Shape sibling %i (Type: %s): 0x%p", shapeSiblingCount, entityTypeStr[(int)currentShapeSibling->Type-1], currentShapeSibling);
			currentShapeSibling = currentShapeSibling->pSibling;
		}
		if (shapeSiblingCount == 10) {
			ImGui::Text("...");
		}

		ImGui::Spacing();

		ImGui::Text("Body: 0x%p (0x%p)", cbody, (TDBody*)((uintptr_t)cbody + (uintptr_t)0x28));
		Entity* currentBodySibling = cbody->pSibling;
		int bodySiblingCount = 0;
		while (currentBodySibling != 0x00 && bodySiblingCount<10) {
			bodySiblingCount++;
			ImGui::Text("Body sibling %i (Type: %s): 0x%p", bodySiblingCount, entityTypeStr[(int)currentShapeSibling->Type-1], currentBodySibling);
			currentBodySibling = currentBodySibling->pSibling;
		}
		if (bodySiblingCount == 10) {
			ImGui::Text("...");
		}

		ImGui::End();
	}

	return 0x01;
}
