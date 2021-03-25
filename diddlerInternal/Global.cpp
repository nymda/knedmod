#include "global.h"
#include "TDObjects.h"

namespace glb {
	TDObjectList* TDOL;
	TDPlayer* player;
	TDGame* game;
	TDRenderer* renderer;
	TDScene* scene;

	cameraPositioning oCamPos;
	damageObject oDamageObject;
	modApiLinker oMAL;
	makeHole oMakeHole;
	damagePlayer oDamagePlayer;
	idfk killme;
	tPaint oPaint;
	tFire oFire;
	tSpawnVox oSpawnVox;
	CreateTexture oCreateTexture;
	CreatePhysics oCreatePhysics;
	UpdateShapes oUpdateShapes;
	B_Constructor oB_Constructor;
	S_Constructor oS_Constructor;
	SetDynamic oSetDynamic;
	TMalloc oTMalloc;
	frameDrawLine oFDL;
	rayCast oRC;
	damageStuff oDamageStuff;
	spawnFire oSpawnFire;
	pewpew oPewpew;

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
}


