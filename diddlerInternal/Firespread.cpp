#include "mem.h"
#include "Firespread.h"
#include <iostream>

namespace fireSpreadMod {
	const byte ignorePlayerRules1[] = { 0x75, 0x59, 0x44, 0x0F, 0x2F, 0x44, 0x37, 0x14, 0x73, 0x51 }; //signature for NOP position 1
	const byte ignorePlayerRules2[] = { 0x75, 0x5F, 0xF3, 0x0F, 0x10, 0x05, 0x91, 0xB7, 0x21, 0x00 }; //signature for NOP position 2
	const byte ignorePlayerRules3[] = { 0x0F, 0x85, 0x05, 0x01, 0x00, 0x00, 0x66, 0xC7  };            //signature for NOP position 3

	const byte normalByteCode1[] =    { 0x75, 0x59 };												  //default data for position 1
	const byte normalByteCode2[] =    { 0x75, 0x5F };												  //default data for position 2
	const byte normalByteCode3[] =    { 0x0F, 0x85, 0x04, 0x01, 0x00, 0x00 };						  //default data for position 3

	byte* ptr_playerRules1 = 0;
	byte* ptr_playerRules2 = 0;
	byte* ptr_playerRules3 = 0;

	bool isFireModEnabled = false;

	void enableEverythingBurns() {
		ptr_playerRules1 = (byte*)mem::FindPattern((byte*)ignorePlayerRules1, "xxxxxxxxxx", GetModuleHandle(NULL));
		ptr_playerRules2 = (byte*)mem::FindPattern((byte*)ignorePlayerRules2, "xxxxxxxxxx", GetModuleHandle(NULL));
		ptr_playerRules3 = (byte*)mem::FindPattern((byte*)ignorePlayerRules3, "xxxxxxxx", GetModuleHandle(NULL));
		
		printf_s("HH: %p, %p, %p\n", ptr_playerRules1, ptr_playerRules2, ptr_playerRules3);

		mem::Nop(ptr_playerRules1, 2);
		mem::Nop(ptr_playerRules2, 2);
		mem::Nop(ptr_playerRules3, 6);

		isFireModEnabled = true;
	}

	void disableEverythingBurns() {
		mem::Patch(ptr_playerRules1, (byte*)normalByteCode1, 2);
		mem::Patch(ptr_playerRules2, (byte*)normalByteCode2, 2);
		mem::Patch(ptr_playerRules3, (byte*)normalByteCode3, 6);

		isFireModEnabled = false;
	}

	void toggleFireMod() {
		if (isFireModEnabled) {
			printf_s("Disabled fire mod\n");
			disableEverythingBurns();
		}
		else {
			printf_s("Enabled fire mod\n");
			enableEverythingBurns();
		}
	}
}