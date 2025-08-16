#pragma once
#include "Entity/Meshes/Meshes.h"

namespace Aura
{
	class ARenderer;
	class EntityManager; // Forward declaration for EntityManager
}

extern Aura::EntityManager AEntityManager;
extern Aura::ARenderer Renderer; // Global instance of ARenderer
