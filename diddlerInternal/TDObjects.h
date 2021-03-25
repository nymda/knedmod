#pragma once
#include "Global.h"
#include "TDFuncs.h"

class TDShape {
public:
	byte paddingA[0x44];
	td::Vec3 min;
	td::Vec3 max;
};

class TDBody {
public:
	byte paddingA[0x28];
	td::Vec3 positionC0;
	td::Vec4 rotationQuatC0;
	td::Vec3 positionC1;
	td::Vec4 rotationQuatC1;
	td::Vec3 positionC2;
	td::Vec4 rotationQuatC2;
};

class TDVox {

};

class TDObjectList {
public:
	uintptr_t objs[128];
};

class TDGame {

};

class TDPlayer {
public:
	td::Vec3 position;			//0x000 - 0x012
	byte paddingA[0x2C];		//0x012 - 0x038
	td::Vec3 velocity;			//0x038 - 0x04A
	byte paddingB[0x36];		//0x04A - 0x07C
	td::Vec3 cameraPosition;	//0x07C - 0x088
	td::Vec4 cameraQuat;		//0x088 - 0x098
	byte paddingC[0x3C];		//0x098 - 0x0D4
	float FMove;				//0x0D4 - 0x0D8
	float SMove;				//0x0D8 - 0x0DC
	byte paddingD[0xD];			//0x0DC - 0x0EA
	byte isAttacking;			//0x0EA - 0x0EB
	byte paddingE[0x72];		//0x0EB - 0x15C
	float health;				//0x15C - 0x160
	byte paddingF[0x268];		//0x160 - 0x3CB
	char heldItemName[13];		//0x3CB - 0x3D8

	td::Vec3 cameraEuler() {
		float vecX = -(2 * (cameraQuat.x * cameraQuat.z + cameraQuat.w * cameraQuat.y));
		float vecY = -(2 * (cameraQuat.y * cameraQuat.z - cameraQuat.w * cameraQuat.x));
		float vecZ = -(1 - 2 * (cameraQuat.x * cameraQuat.x + cameraQuat.y * cameraQuat.y));
		return { vecX, vecY, vecZ };
	}
};

class TDRenderer {

};

class TDScene {

};