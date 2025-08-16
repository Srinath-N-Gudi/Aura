#pragma once
#include "../AuraAPI.h"
#include <unordered_map>
#include<vector>
#include "Entitiy.h"
namespace Aura
{
	class ARenderer; // Forward declaration for ARenderer
	class AURA_API EntityManager
	{
	private:
		std::unordered_map<int, Entity*> m_Entities; // Maps entity ID to Entity pointer
		unsigned int m_NextID = 0; // ID counter for new entities
	public:
		EntityManager();
		~EntityManager();

        friend ARenderer;
// Adds an entity to the manager with a unique ID
    void addEntity(Entity* entity);

    // Removes an entity by its ID
    void removeEntity(int id);

    // Finds an entity by its ID
    Entity* findEntityByID(int id) const;

    // Finds an entity by its name
    Entity* findEntityByName(const std::string& name) const;

    // Finds all entities with a specific tag
    std::vector<Entity*> findEntitiesByTag(const std::string& tag) const;

   // Clears all entities and resets the ID counter
    void clearEntities();

    // Gets the ID of an entity by its name
    unsigned int getIDByName(const std::string& name) const;

    // Gets all IDs of entities with a specific tag
    std::vector<unsigned int> getIDsByTag(const std::string& tag) const;



		inline int getEntityCount() const { return static_cast<int>(m_Entities.size()); }
	};

}
