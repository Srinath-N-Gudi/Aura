#pragma once
#include "../AuraAPI.h"
#include "../Entity/AEntityManager.h"

namespace Aura
{
	namespace Scene
	{
		class ASceneManager;
		class AURA_API AScene
		{
		private:
			Aura::AEntityManager m_EntityManager;
			int m_ID;
		public:
			AScene();
			virtual ~AScene();
			inline Aura::AEntityManager& getEntityManager() { return m_EntityManager; }

			inline int getID() const { return m_ID; } 
			template <typename ObjectType >
			ObjectType* AddObject(const std::string& name, std::string tag = "object");

			friend class ASceneManager;
		};
	}
} 
