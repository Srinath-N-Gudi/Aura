#pragma once

#ifdef AURA_EXPORT
#define AURA_API __declspec(dllexport)
#else
#define AURA_API __declspec(dllimport)
#endif

