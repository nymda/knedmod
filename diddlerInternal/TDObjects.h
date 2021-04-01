#pragma once
#include "Global.h"
#include "TDFuncs.h"

//thanks nahu for the entity structures

enum class entityType : int8_t
{
	Body = 1,
	Shape = 2,
	Light = 3,
	Location = 4,
	Water = 5,
	Enemy = 6,
	Joint = 7,
	Vehicle = 8,
	Wheel = 9,
	Screen = 10,
	Trigger = 11,
	Script = 12
};

static const char* entityTypeStr[] = {
	"Body",
	"Shape",
	"Light",
	"Location",
	"Water",
	"Enemy",
	"Joint",
	"Vehicle",
	"Wheel",
	"Screen",
	"Trigger",
	"Script"
};

class Entity
{
public:
	entityType	Type;			//0x08
	int8_t		Flags;			//0x09
	char		pad_000A[2]; //0x000A
	int16_t		Id; //0x000C
	char		pad_000E[2];	//0x0E
	Entity* pParent;		//0x10
	Entity* pSibling;		//0x18
	Entity* pChild;			//0x20

	virtual void	Destroy(Entity* entity, bool freeMemory);
	virtual Entity* SetParent(Entity* entity, Entity* parent);
	virtual void	Function2();
	virtual void	Function3();
}; // Size: 0x28

class TDVox
{
public:
	int32_t sizeX; //0x0000
	int32_t sizeY; //0x0004
	int32_t sizeZ; //0x0008
	char pad_000C[4]; //0x000C
	void* MaterialBuffer; //0x0010
	void* PhysicsBuffer; //0x0018
	float Scale; //0x0020
	char pad_0024[56]; //0x0024
	int32_t VoxelCount; //0x005C
}; //Size: 0x0060

class TDShape : public Entity
{
public:
	char pad_0019[8]; //0x0028
	byte attributes;
	char pad_008[19];
	td::Vec3 posMin; //0x0044
	td::Vec3 posMax; //0x0050
	char pad_005C[4]; //0x005C
	void* pDormantSomething; //0x0060
	void* pActiveSomething; //0x0068
	char pad_0070[32]; //0x0070
	TDVox* pVox; //0x0090
	char pad_0098[8]; //0x0098
	void* pJoints; //0x00A0 
	float Intergrity; //0x00A8
	bool isBroken; //0x00AC
	char pad_00AD[3]; //0x00AD
}; //Size: 0xB0

class TDBody : public Entity
{
public:
	td::Vec3 Position; //0x0028
	td::Vec4 Rotation; //0x0034
	td::Vec3 Position2; //0x0044
	td::Vec4 Rotation2; //0x0050
	td::Vec3 Position3; //0x0060
	td::Vec4 Rotation3; //0x006C
	td::Vec3 Velocity; //0x007C
	td::Vec3 RotationVelocity; //0x0088
	char pad_0094[12]; //0x0094
	float Lightness; //0x00A0
	char pad_00A4[24]; //0x00A4
	float Integrity; //0x00BC
	td::Vec3 N00006C83; //0x00C0
	td::Vec3 N00006C80; //0x00CC
	float N00002F17; //0x00D8
	bool Dynamic; //0x00DC
	char pad_00DD[8]; //0x00DD
	bool Colliding; //0x00E5
	char pad_00E6[2]; //0x00E6
}; //Size: 0xE8

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