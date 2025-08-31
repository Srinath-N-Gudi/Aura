
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
        static AEntityManager& Get();

        // Public interface functions (user calls these)
        static void addEntity(Entity* entity);
        static void removeEntity(int id);
        static Entity* findEntityByID(int id);
        static Entity* findEntityByName(const std::string& name) ;
        static std::vector<Entity*> findEntitiesByTag(const std::string& tag) ;
        static void clearEntities();
		static unsigned int getEntityCount() { return Get().m_Entities.size(); }
        static unsigned int getIDByName(const std::string& name) ;
        static std::vector<unsigned int> getIDsByTag(const std::string& tag) ;
        friend class ARenderer;
    private:
        // Private constructor/destructor for singleton
        AEntityManager();
        ~AEntityManager();

        // Delete copy/move constructors
        AEntityManager(AEntityManager&&) = delete;
        AEntityManager& operator=(AEntityManager&&) = delete;

        // Internal functions (actual logic)
        void IaddEntity(Entity* entity);
        Entity* IfindEntityByID(int id) const;
        Entity* IfindEntityByName(const std::string& name) const;
        std::vector<Entity*> IfindEntitiesByTag(const std::string& tag) const;
        void IclearEntities();
        unsigned int IgetIDByName(const std::string& name) const;
        std::vector<unsigned int> IgetIDsByTag(const std::string& tag) const;

        // Internal data
        std::unordered_map<unsigned int, Entity*> m_Entities;
		
        static std::unordered_map<unsigned int, Entity*>& Entities() {
            return Get().m_Entities;
        }
        unsigned int m_NextID = 0;
    };
}
