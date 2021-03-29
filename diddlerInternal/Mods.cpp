#include "Jetpack.h"
#include "Flamethrower.h"
#include "Global.h"
#include "cutter.h"
#include "objectSpawner.h"
#include "noclip.h"
#include "c4.h"

namespace mods {
	char jetpackKey = VK_SPACE;
	char noclipKey = 0x56;

	bool jetpack = false;
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
		c4::runC4();
	}
}