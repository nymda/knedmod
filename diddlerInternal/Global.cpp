#include "global.h"
#include "TDObjects.h"

namespace glb {
	TDObjectList* TDOL;
	TDPlayer* player;
	TDGame* game;
	TDRenderer* renderer;
	TDScene* scene;
	tdMain oTDmain;
	movementLoop oMovLoop;

	outlineShape oOutlineshape;
	SetObjectAttribute oSOA;
	spreadFire oSpreadFire;
	environmentUpdate oEnvUpdate;
	createLight oCreateLight;
	deleteBody oTdDelBdy;
	cameraPositioning oCamPos;
	damageObject oDamageObject;
	modApiLinker oMAL;
	makeHole oMakeHole;
	damagePlayer oDamagePlayer;
	idfk killme;
	tPaint oPaint;
	tSpawnVox oSpawnVox;
	CreateTexture oCreateTexture;
	CreatePhysics oCreatePhysics;
	UpdateShapes oUpdateShapes;
	B_Constructor oB_Constructor;
	S_Constructor oS_Constructor;
	SetDynamic oSetDynamic;
	TMalloc oTMalloc;
	TFree oTFree;
	frameDrawLine oFDL;
	rayCast oRC;
	spawnFire oSpawnFire;
	createProjectile oPewpew;

	createExplosion TDcreateExplosion;
	spawnParticle TDspawnParticle;

	createExplosionWrapped TDcreateExplosionWrapped;
	spawnParticleWrapped TDspawnParticleWrapped;

	uintptr_t moduleBase;
	twglSwapBuffers owglSwapBuffers;
	bool displayMenu = false;
	HWND gWnd;
	hk_SetCursorPos ocursor;
	HMODULE hMdl;
	uintptr_t plankPatchFunction;
}


