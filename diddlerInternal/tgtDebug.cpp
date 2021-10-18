#include "tgtDebug.h"
#include "Raycaster.h"
#include "types.h"

tgt_debug::tgt_debug() {

}

DWORD tgt_debug::exec() {
	td::Color green{ 0.f, 1.f, 0.f, 1.f };
	td::Color blue{ 0.f, 0.f, 1.f, 1.f };

	raycaster::rayData rayDat = raycaster::castRayPlayer();
	if (rayDat.hitShape) {
		glb::oOutlineShape(glb::renderer, rayDat.hitShape, &green, 1.f);
		dbgShape = rayDat.hitShape;
		dbgBody = rayDat.hitShape->getParentBody();
	}
	return 0x01;
}
