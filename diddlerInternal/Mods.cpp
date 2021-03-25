#include "Jetpack.h"
#include "Flamethrower.h"
#include "Global.h"
#include "cutter.h"
#include "objectSpawner.h"
#include "noclip.h"

namespace mods {
	char jetpackKey = VK_SPACE;

	bool jetpack = true;
	bool flamethrower = true;
	bool godmode = true;

	void execMods() {
		spawner::processMostRecentObject();

		if (jetpack) {
			jetpack::executeJetpack();
		}
		if (flamethrower) {
			flamethrower::execFlamethrower();
		}
		spawner::handleSpawnerWeapon();
		cutter::runCutter();
		noclip::update();
	}
}