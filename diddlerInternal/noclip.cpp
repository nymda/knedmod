#include "Global.h"
#include "mem.h"
#include "maths.h"

namespace noclip {
	bool enabled = false;
	bool inNoclip = false;
	bool setup = false;
	byte xBuffer[5] = {};
	byte yBuffer[8] = {};
	byte zBuffer[8] = {};

	td::Vec3 movementVector = { 0, 0, 0 };
	td::Vec3 lastMmovementVector = { 0, 0, 0 };
	td::Vec3 cameraForwardVector = { 0, 0, 0 };
	td::Vec3 cameraSideVector = { 0, 0, 0 };
	td::Vec4 cameraQuat = { 0, 0, 0, 0 };

	void backupInstructions() {
		uintptr_t baseAddy = (uintptr_t)glb::oCamPos;
		uintptr_t xInstruction = baseAddy + 0x629; // ok
		uintptr_t yInstruction = baseAddy + 0x667; // ok 
		uintptr_t zInstruction = baseAddy + 0x62E; // ok

		memcpy(xBuffer, (void*)xInstruction, 5);
		memcpy(yBuffer, (void*)yInstruction, 8);
		memcpy(zBuffer, (void*)zInstruction, 8);

		setup = true;
	}

	void ToggleNoclip() {
		uintptr_t baseAddy = (uintptr_t)glb::oCamPos;
		uintptr_t xInstruction = baseAddy + 0x629; // ok
		uintptr_t yInstruction = baseAddy + 0x667; // ok 
		uintptr_t zInstruction = baseAddy + 0x62E; // ok

		inNoclip = !inNoclip;
		if (inNoclip) {
			if (!setup) { backupInstructions(); }
			mem::Nop((byte*)xInstruction, 5); 
			mem::Nop((byte*)yInstruction, 8);
			mem::Nop((byte*)zInstruction, 8);
		}
		else {
			mem::Patch((byte*)xInstruction, xBuffer, 5);
			mem::Patch((byte*)yInstruction, yBuffer, 8);
			mem::Patch((byte*)zInstruction, zBuffer, 8);

			glb::player->position = { glb::player->cameraPosition.x, glb::player->cameraPosition.y - 1.7f, glb::player->cameraPosition.z };

			glb::player->velocity = { lastMmovementVector.x * 50, lastMmovementVector.y * 50, lastMmovementVector.z * 50 };
		}
	}

	float speed = 0.5;
	void update() {
		if (!inNoclip) {
			return;
		}

		cameraQuat = glb::player->cameraQuat;
		td::Vec3* camPos = &glb::player->cameraPosition;
		float* sm = &glb::player->SMove;
		float* fm = &glb::player->FMove;

		movementVector = { 0, 0, 0 };

		cameraForwardVector.x =		2 * (cameraQuat.x * cameraQuat.z + cameraQuat.w * cameraQuat.y);
		cameraForwardVector.y =		2 * (cameraQuat.y * cameraQuat.z - cameraQuat.w * cameraQuat.x);
		cameraForwardVector.z = 1 - 2 * (cameraQuat.x * cameraQuat.x + cameraQuat.y * cameraQuat.y);

		cameraSideVector.x =	1 - 2 * (cameraQuat.y * cameraQuat.y + cameraQuat.z * cameraQuat.z);
		cameraSideVector.y =		2 * (cameraQuat.x * cameraQuat.y + cameraQuat.w * cameraQuat.z);
		cameraSideVector.z =		2 * (cameraQuat.x * cameraQuat.z - cameraQuat.w * cameraQuat.y);

		if(*fm == 1.f) {
			movementVector.x += -(cameraForwardVector.x);
			movementVector.y += -(cameraForwardVector.y);
			movementVector.z += -(cameraForwardVector.z);
		}
		if (*fm == -1.f) {
			movementVector.x -= -(cameraForwardVector.x);
			movementVector.y -= -(cameraForwardVector.y);
			movementVector.z -= -(cameraForwardVector.z);
		}
		if (*sm == 1.f) {
			movementVector.x += (cameraSideVector.x);
			movementVector.y += (cameraSideVector.y);
			movementVector.z += (cameraSideVector.z);
		}
		if (*sm == -1.f) {
			movementVector.x -= (cameraSideVector.x);
			movementVector.y -= (cameraSideVector.y);
			movementVector.z -= (cameraSideVector.z);
		}

		if (((GetAsyncKeyState(VK_SHIFT) >> 15) & 0x0001) == 0x0001) {
			speed = 0.5f; //fast
		}
		else if (((GetAsyncKeyState(VK_CONTROL) >> 15) & 0x0001) == 0x0001) {
			speed = 5.0f; //slow
		}
		else {
			speed = 3.0f; //normal
		}

		movementVector.x = movementVector.x / speed;
		movementVector.y = movementVector.y / speed;
		movementVector.z = movementVector.z / speed;

		if (*fm != 0 || *sm != 0) {
			camPos->x += movementVector.x;
			camPos->y += movementVector.y;
			camPos->z += movementVector.z;
		}

		glb::player->position = { 0, 500, 0 };
		glb::player->velocity = { 0, 0, 0 };

		lastMmovementVector = movementVector;

	}
}