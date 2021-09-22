#include <chrono>
#include <thread>
#include "constClock.h"
#include <iostream>
#include <string>
#include "windows.h"

namespace constClock {
	std::thread clockThread;
	std::chrono::high_resolution_clock execTimer;
	bool terminator = true;

	std::chrono::steady_clock::time_point tp1;
	std::chrono::steady_clock::time_point tp2;

	void beginConstantClock(float pulseLengthMS) {
		clockThread = std::thread(cUpdateThread, pulseLengthMS, &terminator);
		clockThread.detach();
	}

	void endConstantClock() {
		terminator = false;
	}

	void cUpdateThread(float msTime, bool* terminator) {
		while (true) {
			if (*terminator) {
				auto startTime = execTimer.now();
				cUpdateFunction();
				auto endTime = execTimer.now();
				float execTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1000.f;

				if (execTime < msTime) {
					Sleep(msTime - execTime);
				}
			}
			else {
				return;
			}
		}
	}

	//this function runs at a constant speed, depending on the input mstime
	void cUpdateFunction() {
		tp1 = tp2;
		tp2 = execTimer.now();
		std::cout << "Pulse time: " << std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count() / 1000.f) << std::endl;
	}
}