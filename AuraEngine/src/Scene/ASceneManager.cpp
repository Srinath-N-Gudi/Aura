#include "ASceneManager.h"
#include "../AuraLogger.h"

namespace Aura
{
	namespace Scene
	{
		void ASceneManager::IaddScene(AScene* scene)
		{
			if (!scene) return;
			scene->m_ID = m_NextID++;
			m_Scenes.push_back(scene);
		}
		void ASceneManager::AddScene(AScene* scene)
		{
			Get().IaddScene(scene);
		}
		ASceneManager::~ASceneManager()
		{
			for (auto scene : m_Scenes) {
				if (scene)
				delete scene;
			}
			m_Scenes.clear();
		}
		AScene* ASceneManager::GetScene(int id) {
			if (id < 0 || id >= static_cast<int>(Get().m_Scenes.size())) {
				AURA_CORE_ERROR("[ASceneManager] : Invalid scene ID: {}", id);
				return nullptr;
			}
			return Get().m_Scenes[id];
		}
		AScene* ASceneManager::GetScene()
		{
			int activeID = Get().m_ActiveScene;
			if (activeID == -1 && Get().m_Scenes.size() >= 1) {
				AURA_CORE_WARN("[ASceneManager] : No active scene set! Defaulting to scene id 0");
				SetActiveScene(0);
			}
			else if (activeID >= static_cast<int>(Get().m_Scenes.size())) {
				AURA_CORE_ERROR("[ASceneManager] : Invalid Active Scene ID", activeID);
				return nullptr;
			}
			return Get().m_Scenes[activeID==-1?0:activeID];
		}

	}
}
