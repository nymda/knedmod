#include "Global.h"
#include "raycaster.h"

namespace cutter {
	void runCutter() {
		td::Vec4 position = castRayPlayer();
		int counter = 0;
		td::Vec3 pSmall = { position.x, position.y, position.z };

		//glb::TDcreateExplosionWrapped(1.l, &pSmall, 1.f);

		//std::cout << glb::oMakeHole(&pSmall, 4, 4, 4, true) << std::endl;
	}
}
