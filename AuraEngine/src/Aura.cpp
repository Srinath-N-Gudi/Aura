#include "AuraLogger.h"
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // Runs when the DLL is loaded by a process
        std::cout << "AuraEngine DLL loaded!" << std::endl;
        Aura::Logger::Init();
        break;
    case DLL_PROCESS_DETACH:
        // Runs when the DLL is unloaded
        std::cout << "AuraEngine DLL unloaded!" << std::endl;
        break;
    }
    return TRUE;
}