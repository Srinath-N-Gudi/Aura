#include "AEntityManager.h"
#include <algorithm>

namespace Aura
{
   // Constructor / Destructor
   AEntityManager::AEntityManager() = default;
   AEntityManager::~AEntityManager()
   {
       clearEntities(); // Clear all entities on destruction
   }

   // Internal() implementations (actual logic)
   void AEntityManager::addEntity(Entity* entity)
   {
       if (!entity) return;
       m_Entities[m_NextID] = entity;
       entity->m_ID = m_NextID++;
   }

   Entity* AEntityManager::findEntityByID(int id) const
   {
       auto it = m_Entities.find(id);
       return (it != m_Entities.end()) ? it->second : nullptr;
   }

   Entity* AEntityManager::findEntityByName(const std::string& name) const
   {
       for (const auto& pair : m_Entities)
       {
           if (pair.second->getName() == name)
               return pair.second;
       }
       return nullptr;
   }

   std::vector<Entity*> AEntityManager::findEntitiesByTag(const std::string& tag) const
   {
       std::vector<Entity*> result;
       for (const auto& pair : m_Entities)
       {
           if (pair.second->getTag() == tag)
               result.push_back(pair.second);
       }
       return result;
   }

   void AEntityManager::clearEntities()
   {
	   for (auto& pair : m_Entities)
	   {
	   AURA_CORE_INFO("ENTITY COUNT : {} ", m_Entities.size());
		   delete pair.second; // Free memory
	   }
       m_Entities.clear();
       m_NextID = 0;
   }

   unsigned int AEntityManager::getIDByName(const std::string& name) const
   {
       for (const auto& pair : m_Entities)
       {
           if (pair.second->getName() == name)
               return pair.first;
       }
       return 0; // Not found
   }

   std::vector<unsigned int> AEntityManager::getIDsByTag(const std::string& tag) const
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
