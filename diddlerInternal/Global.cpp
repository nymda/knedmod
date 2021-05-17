#include "global.h"
#include "TDObjects.h"


namespace glb {

	void setObjectAttribute(TDShape* shape, const char* a1, const char* a2) {
		td::small_string p1 = td::small_string(a1);
		td::small_string p2 = td::small_string(a2);
		glb::oSOA(shape, &p1, &p2);
	}

	TDObjectList* TDOL;
	TDPlayer* player;
	TDGame* game;
	TDRenderer* renderer;
	TDScene* scene;
	tdMain oTDmain;
	wrappedDoDamage oWrappedDamage;
	setBodySomething oSetBody;
	ConstructJoint oConstructJoint;
	AttachJoint oAttachJoint;
	handleBombObjs oHandleBombObjs;
	setShapeParentBody oSetShapeParentBody;
	unknReadVoxData oIUnReadVox;

	highlightShape oHighlightShape;
	outlineShape oOutlineShape;
	outlineBody oOutlineBody;
	SetObjectAttribute oSOA;
	SetObjectAttributeSingle oSOAS;
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
	ReadSubobjectsFromVox TDreadSubobjects;

	apiQuatEuler oQuatEul;
	addContextItem oAddCItem;

	uintptr_t moduleBase;
	twglSwapBuffers owglSwapBuffers;
	bool displayMenu = false;
	HWND gWnd;
	hk_SetCursorPos ocursor;
	HMODULE hMdl;
	uintptr_t plankPatchFunction;
}


