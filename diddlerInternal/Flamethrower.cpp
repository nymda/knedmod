#include "Global.h"
#include "Raycaster.h"
#include "drawCube.h"

namespace flamethrower {
	float flRadius = 1.5f;

	void execFlamethrower() {
		const char* name = "blowtorch";

		if (memcmp(glb::player->heldItemName, name, 9) == 0) {
			if (glb::player->isAttacking == true) {
				td::Vec4 target = castRayPlayer();
				td::Color red{ 1.f, 0.f, 0.f, 1.f };
				drawCube({ target.x, target.y, target.z }, flRadius, red);

				for (float ty = (target.y - flRadius); ty < (target.y + flRadius); ty += 0.1f) {
					for (float tx = (target.x - flRadius); tx < (target.x + flRadius); tx += 0.1f) {
						for (float tz = (target.z - flRadius); tz < (target.z + flRadius); tz += 0.1f) {
							if ((rand() % 100) == 0) {
								td::Vec3 firepos = { tx, ty, tz };
								glb::oSpawnFire(*(uintptr_t*)(glb::scene + 0xA8), &firepos);
							}
						}
					}
				}
			}
			else {
				td::Vec4 target = castRayPlayer();
				td::Color green{ 0.f, 1.f, 0.f, 1.f };
				drawCube({ target.x, target.y, target.z }, flRadius, green);
			}
		}
	}
}
