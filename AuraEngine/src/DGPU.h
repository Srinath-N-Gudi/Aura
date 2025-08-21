#pragma once
#include <Windows.h>
// For NVIDIA Optimus
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	// For AMD PowerXpress
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
