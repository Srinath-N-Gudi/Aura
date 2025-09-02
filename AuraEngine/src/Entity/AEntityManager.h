
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../AuraAPI.h"`
#include "Entity.h"

class Entity; // Forward declaration of your Entity class

namespace Aura
{
    class ARenderer;
    class AURA_API AEntityManager
    {
    public:
        // Singleton access
		 AEntityManager();
        ~AEntityManager();
        /// Internal() functions (actual logic)
        void addEntity(Entity* entity);
        Entity* findEntityByID(int id) const;
        Entity* findEntityByName(const std::string& name) const;
        std::vector<Entity*> findEntitiesByTag(const std::string& tag) const;
        void clearEntities();
        unsigned int getIDByName(const std::string& name) const;
        std::vector<unsigned int> getIDsByTag(const std::string& tag) const;

        friend class ARenderer;


    private:

        // Delete copy/move constructors
        AEntityManager(AEntityManager&&) = delete;
        AEntityManager& operator=(AEntityManager&&) = delete;
        std::unordered_map<unsigned int, Entity*> m_Entities;
        unsigned int m_NextID = 0;
    };
}
