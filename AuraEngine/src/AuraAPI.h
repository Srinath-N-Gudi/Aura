#pragma once
#ifdef BUILD_AURA_DLL
#ifdef AURA_EXPORT
#define AURA_API __declspec(dllexport)
#else
#define AURA_API __declspec(dllimport)
#endif
#else
#define AURA_API
#endif
