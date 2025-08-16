#include "EntityManager.h"
namespace Aura
{
	EntityManager::EntityManager() = default;
	EntityManager::~EntityManager() {
		clearEntities(); // Clear all entities on destruction
	}
	void EntityManager::addEntity(Entity* entity)
	{
		if (entity == nullptr) return; // Avoid EntityManager::adding null entities
		m_Entities[m_NextID] = const_cast<Entity*>(entity); // Store entity with unique ID
		entity->m_ID = m_NextID++; // Set the entity's ID
	}

	void EntityManager::removeEntity(int id)
	{
		auto it = m_Entities.find(id);
		if (it != m_Entities.end())
		{
			delete it->second; // Free memory if needed
			m_Entities.erase(it); // Remove from map
		}
	}
	Entity* EntityManager::findEntityByID(int id) const
	{
		auto it = m_Entities.find(id);
		Entity* outEntity;
		if (it != m_Entities.end())
		{
			outEntity = it->second; // Set output entity pointer
	}
		else
		{
			outEntity = nullptr; // Not found
		}
		return outEntity;
	}
	Entity* EntityManager::findEntityByName(const std::string& name) const
	{
		Entity* outEntity = nullptr;
		for (const auto& pair : m_Entities)
		{
			if (pair.second->getName() == name)
			{
				outEntity = pair.second; // Set output entity pointer
			}
		}
		return outEntity;
	}

	std::vector<Entity*> EntityManager::findEntitiesByTag(const std::string& tag) const
	{
		std::vector<Entity*> outEntities;
		outEntities.clear(); // Clear output vector
		for (const auto& pair : m_Entities)
		{
			if (pair.second->getTag() == tag)
			{
				outEntities.push_back(pair.second); // Add matching entity to output vector
			}
		}
		return outEntities;
	}

	void EntityManager::clearEntities()
	{
		for (auto& pair : m_Entities)
		{
			delete pair.second; // Free memory for each entity
		}
		m_Entities.clear(); // Clear the map
		m_NextID = 0; // Reset ID counter
	}

	unsigned int EntityManager::getIDByName(const std::string& name) const {
		unsigned int outID = 0;
		for (const auto& pair : m_Entities)
		{
			if (pair.second->getName() == name)
			{
				outID = pair.first; // Set output ID
			}
		}
		return outID;
	}
	std::vector<unsigned int> EntityManager::getIDsByTag(const std::string& tag) const
	{
		std::vector<unsigned int> outIDs;
		outIDs.clear(); // Clear output vector
		for (const auto& pair : m_Entities)
		{
			if (pair.second->getTag() == tag)
			{
				outIDs.push_back(pair.first); // Add matching entity ID to output vector
			}
		}
		return outIDs;
	}
}



Aura::EntityManager AEntityManager;
