#include "Jetpack.h"
#include "Flamethrower.h"
#include "Global.h"
#include "cutter.h"
#include "objectSpawner.h"
#include "noclip.h"
#include "c4.h"
#include "camera.h"
#include "toolgun.h"
#include "lantern.h"
#include "missileLauncher.h"

namespace mods {
	char jetpackKey = VK_SPACE;
	char noclipKey = 0x56;

	bool jetpack = false;
	bool flamethrower = true;
	bool godmode = true;
	bool c4_global_detonation = false;
	bool removeWalls = false;

	void execMods() {
		spawner::processMostRecentObject();
		toolgun::handleToolgun();
		c4::runC4();
		lantern::updateLantern();
		smoker::updateSmoker();
		missile::runMissile();

		if (jetpack) {
			jetpack::executeJetpack();
		}

		if (removeWalls) {
			
			int count = 0;

			if (*(byte*)&(glb::scene->Boundaries) != 0x00) {
				for (td::Vec2* pt : glb::scene->Boundaries) {
					count++;
				}
				mem::Null((byte*)&(glb::scene->Boundaries), 8 * count);
			}


		}

		//if (flamethrower) {
		//	flamethrower::execFlamethrower();
		//}
		//spawner::handleSpawnerWeapon();

		//camera::runCamera();
	}
}