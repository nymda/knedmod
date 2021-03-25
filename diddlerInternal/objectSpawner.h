#pragma once
#include <string>
#include "types.h"
#include "Global.h"
#include "Raycaster.h"

inline bool exists(const std::string& name);

namespace spawner {

	extern std::string currentSpawngunObject;

	struct KMSpawnedObject {
		bool isInitByGame = false;
		TDShape* shape = 0;
		TDBody* body = 0;
	};
#
	struct LoadedSpawnableObject {
		std::string basePath;
		std::string voxPath;
		std::string imagePath;
		GLuint imageTexture = 0;
	};

	void spawnObjectProxy(std::string path);
	void processMostRecentObject();
	std::vector<LoadedSpawnableObject> enumerateSpawnableObjects();
	void handleSpawnerWeapon();
	KMSpawnedObject spawnEntity(std::string filepath);
	void spawnTestEntity();
}

