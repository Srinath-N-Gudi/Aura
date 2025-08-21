
#include "AEntityManager.h"
#include <algorithm>

namespace Aura
{
    // Singleton access
    AEntityManager& AEntityManager::Get()
    {
        static AEntityManager instance;
        return instance;
    }

    // Constructor / Destructor
    AEntityManager::AEntityManager() = default;

    AEntityManager::~AEntityManager()
    {
        clearEntities(); // Clear all entities on destruction
    }

    // Public functions (forward to internal functions)
    void AEntityManager::addEntity(Entity* entity)
    {
        Get().IaddEntity(entity);
    }

    void AEntityManager::removeEntity(int id)
    {
        Get().IremoveEntity(id);
    }

    Entity* AEntityManager::findEntityByID(int id) 
    {
        return Get().IfindEntityByID(id);
    }

    Entity* AEntityManager::findEntityByName(const std::string& name) {
        return Get().IfindEntityByName(name);
    }

    std::vector<Entity*> AEntityManager::findEntitiesByTag(const std::string& tag) 
    {
        return Get().IfindEntitiesByTag(tag);
    }

    void AEntityManager::clearEntities()
    {
        Get().IclearEntities();
    }

    unsigned int AEntityManager::getIDByName(const std::string& name) 
    {
        return Get().IgetIDByName(name);
    }

    std::vector<unsigned int> AEntityManager::getIDsByTag(const std::string& tag) 
    {
        return Get().IgetIDsByTag(tag);
    }

    // Internal implementations (actual logic)
    void AEntityManager::IaddEntity(Entity* entity)
    {
        if (!entity) return;
        m_Entities[m_NextID] = entity;
        entity->m_ID = m_NextID++;
    }

    void AEntityManager::IremoveEntity(int id)
    {
        auto it = m_Entities.find(id);
        if (it != m_Entities.end())
        {
            delete it->second; // Free memory if needed
            m_Entities.erase(it);
        }
    }

    Entity* AEntityManager::IfindEntityByID(int id) const
    {
        auto it = m_Entities.find(id);
        return (it != m_Entities.end()) ? it->second : nullptr;
    }

    Entity* AEntityManager::IfindEntityByName(const std::string& name) const
    {
        for (const auto& pair : m_Entities)
        {
            if (pair.second->getName() == name)
                return pair.second;
        }
        return nullptr;
    }

    std::vector<Entity*> AEntityManager::IfindEntitiesByTag(const std::string& tag) const
    {
        std::vector<Entity*> result;
        for (const auto& pair : m_Entities)
        {
            if (pair.second->getTag() == tag)
                result.push_back(pair.second);
        }
        return result;
    }

    void AEntityManager::IclearEntities()
    {
        for (auto& pair : m_Entities)
        {
            delete pair.second;
        }
        m_Entities.clear();
        m_NextID = 0;
    }

    unsigned int AEntityManager::IgetIDByName(const std::string& name) const
    {
        for (const auto& pair : m_Entities)
        {
            if (pair.second->getName() == name)
                return pair.first;
        }
        return 0; // Not found
    }

    std::vector<unsigned int> AEntityManager::IgetIDsByTag(const std::string& tag) const
    {
        std::vector<unsigned int> ids;
        for (const auto& pair : m_Entities)
        {
            if (pair.second->getTag() == tag)
                ids.push_back(pair.first);
        }
        return ids;
    }
}
