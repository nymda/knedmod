#pragma once
#include <string>
#include "types.h"
#include "Global.h"
#include "Raycaster.h"

inline bool exists(const std::string& name);

namespace spawner {

	extern std::string currentSpawngunObject;

	enum objectSpawnType {
		placed = 0,
		thrown = 1
	};

	struct objectSpawnerParams {
		objectSpawnType spawnType = objectSpawnType::placed;
		td::Vec3 startVelocity = { 0, 0, 0 };
		float maximumPlaceDistance = 1000.f;
		bool rotateFacePlayer = false;
	};

	struct KMSpawnedObject {
		objectSpawnerParams params;
		bool isInitByGame = false;
		TDShape* shape = 0;
		TDBody* body = 0;
	};
	struct LoadedSpawnableObject {
		std::string basePath;
		std::string voxPath;
		std::string imagePath;
		GLuint imageTexture = 0;
	};

	KMSpawnedObject spawnObjectProxy(std::string path, objectSpawnerParams params);
	void processMostRecentObject();
	std::vector<LoadedSpawnableObject> enumerateSpawnableObjects();
	void handleSpawnerWeapon();
	KMSpawnedObject spawnEntity(std::string filepath, objectSpawnerParams osp);
}

