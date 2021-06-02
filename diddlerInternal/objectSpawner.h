#pragma once
#include <string>
#include "types.h"
#include "Global.h"
#include "Raycaster.h"

inline bool exists(const std::string& name);

namespace spawner {

	extern std::string currentSpawngunObject;

	struct objectAttribute {
		td::small_string attribute;
		td::small_string level;
	};

	enum objectSpawnType {
		placed = 0,
		thrown = 1,
		front = 2
	};

	struct objectSpawnerParams {
		TDBody* parentBody = 0;
		objectSpawnType spawnType = objectSpawnType::placed;
		std::vector<objectAttribute> attributes;
		td::Vec3 startVelocity = { 0, 0, 0 };
		td::Vec4 customRotation = { 0, 0, 0 };
		float maximumPlaceDistance = 1000.f;
		bool rotateFacePlayer = false;
		bool unbreakable = false;
		bool nocull = false;
		bool pushSpawnList = true;
		bool animate = false;
	};

	struct KMSpawnedObject {
		objectSpawnerParams params;
		bool isInitByGame = false;
		TDShape* shape = 0;
		TDBody* body = 0;
		TDVox* vox = 0;
	};

	struct LoadedSpawnableObject {
		std::string catagory;
		std::string basePath;
		std::string voxPath;
		std::string imagePath;
		std::string objectName;
		GLuint imageTexture = 0;
		std::vector<objectAttribute> attributes;
	};

	struct spawnerCatagory {
		std::string name;
		std::vector<LoadedSpawnableObject> objects;
	};

	void deleteLastObject();
	KMSpawnedObject spawnObjectProxy(std::string path, objectSpawnerParams params);
	void processMostRecentObject();
	std::vector<spawnerCatagory> enumerateSpawnableObjects();
	void handleSpawnerWeapon();
	KMSpawnedObject spawnEntity(std::string filepath, objectSpawnerParams osp);
}

