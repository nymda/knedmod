#pragma once
#include "Global.h"
#include "TDFuncs.h"

//thanks nahu for the entity structures

enum class gameState : int8_t {
	splash = 1,
	menu = 2,
	start_ui = 3,
	ingame = 4,
	editor = 5,
	quitting = 6
};

static const char* gameStateTXT[] {
	"splash",
	"menu",
	"start_ui",
	"ingame",
	"editor",
	"quitting"
};

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

class Environment
{
public:
	const char* skyboxpath;
	char pad_0008[8]; //0x0008
	td::Vec3 sunColor; //0x0010
	char pad_001C[4]; //0x001C
	float skyboxrot; //0x0020
	char pad_0024[12]; //0x0024
	td::Vec3 sunColorTint; //0x0030
	char pad_003C[16]; //0x003C
	float sunBrightness; //0x004C
	float sunSpread; //0x0050
	float sunLength; //0x0054
	float sunFogScale; //0x0058
	float sunGlare; //0x005C
	char pad_0060[4]; //0x0060
	float ambient; //0x0064
	td::Vec2 exposure; //0x0068
	float brightness;
	td::Vec3 fogColor; //0x0074
	char pad_0080[4]; //0x0080
	td::Vec4 fogParams; //0x0084
	float wetness; //0x0094
	float puddleamount; //0x0098
	float puddlesize; //0x009C
	float rain; //0x00A0
	bool nightlight; //0x00A4
	char pad_00A5[83]; //0x00A5
	td::small_string ambience; //0x00F8
	float slippery; //0x0110
}; //Size: 0x0114

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

class TDJoint : public Entity {
public:
};

class TDObjectList {
public:
	uintptr_t objs[128];
};

class TDGame {
public:
	char pad_0000[8]; 
	gameState State;
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

struct FireSystem {
	struct Fire {
		TDShape* Parent;
		td::Vec3 Position;
		float m_TimeRemaining;
		float m_TotalLifetime;
	};

	void* vftptr_0;
	td::small_vector<Fire> m_Instances;
};

class TDRenderer {

};

class TDLight : Entity {
	enum class Type : uint8_t {
		Sphere = 1,
		Cone = 2,
		Area = 3,
	};

public:
	bool m_Enabled; 
	uint8_t pad29[3];
	Type m_LightType;
	uint8_t pad2D[3];
	td::Vec3 m_Transform;
	td::Vec3 m_Color;
	float dword58;
	float m_Reach;
	float m_Size;
	float m_ShadowDistance;
	float m_InnerAngle;
	float m_OuterAngle;
	float m_Fogiter;
	float m_FogScale;
	td::Vec2 m_AreaSize;
	float m_Glare;
	uint64_t qword84;
	uint64_t qword8C;
	bool byte94; 
	uint8_t pad95[3];
	uint32_t dword98;
	uint16_t word9C; 
	uint8_t pad9D[2];
	uint8_t gap9f[520]; // This is a ShaderBinding instance
	uint8_t qword2A8[56]; // This is a Buffer instance
	uint8_t gap2E0[32]; // This is AnotherBuffer instance
	uint32_t dword300;
	uint32_t dword304;
	uint64_t qword308;
	uint8_t gap310[3072];
	uint32_t dwordF10;
	uint32_t dwordF14;
	uint64_t qwordF18;
	uint8_t gapF20[1024];
	uint64_t qword1320;
	uint64_t qword1328;
	uint64_t qword1330;
	td::small_string m_Sound;
	uint64_t qword1348;
	uint32_t dword1350;
	uint32_t dword1354;
};

class TDScene {
public:
	char pad_0008[8]; //0x0008
	class ppScene* ppScene; //0x0010
	char pad_0018[88]; //0x0018
	void* pParticleSystem; //0x0070
	char pad_0078[48]; //0x0078
	class fireSystem* pFireSystem; //0x00A8
	td::small_vector<void*> projectileList;
	Environment* pEnvironment; //0x00C0
	td::Vec3 spawnPos; //0x00C8
	char pad_00D4[20]; //0x00D4
	TDLight* pFlashlight; //0x00E8
	char pad_00F0[48]; //0x00F0
	class Vehicle* pCurrentVehicle; //0x0120
	class Screen* pCurrentScreen; //0x0128
	char pad_0130[248]; //0x0130
	td::small_vector<Entity*> entityList; //0x0228
	td::small_vector<TDBody*>* pBodyList; //0x0238
	td::small_vector<TDShape*>* pShapeList; //0x0240
	td::small_vector<TDLight*>* pLightList; //0x0248
	td::small_vector<void*>* pLocationList; //0x0250
	td::small_vector<void*>* pWaterList; //0x0258
	td::small_vector<void*>* pEnemyList; //0x0260
	td::small_vector<void*>* pJointList; //0x0268
	td::small_vector<Vehicle*>* pVehicleList; //0x0270
	td::small_vector<void*>* pWheelList; //0x0278
	td::small_vector<void*>* pScreenList; //0x0280
	td::small_vector<void*>* pTriggerList; //0x0288
	td::small_vector<void*>* pScriptList; //0x0290
	char pad_0298[48]; //0x0298
	float distanceToBoundary; //0x02C8
	char pad_02CC[612]; //0x02CC
	td::small_vector<td::Vec2*> Boundaries; //0x0530
	char pad_0540[864]; //0x0540
	td::small_vector<Entity*> entityList2; //0x08A0
	char pad_08B0[8624]; //0x08B0
	void* (*pSomethingArray)[150]; //0x2A60
	char pad_2A68[1792]; //0x2A68

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
};