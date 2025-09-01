#pragma once
#include "../AuraAPI.h"
#include "Scene.h"
#include <vector>
namespace Aura
{
	namespace Scene
	{
		class AURA_API ASceneManager
		{
		private:
			ASceneManager() = default;
			~ASceneManager();
			std::vector<AScene*> m_Scenes;
			int m_NextID = 0;
			int m_ActiveScene = -1;
			void IaddScene(AScene* scene);
		public:
			static ASceneManager& Get() {
				static ASceneManager instance;
				return instance;
			}
			ASceneManager(const ASceneManager&) = delete;
			ASceneManager& operator=(const ASceneManager&) = delete;
			ASceneManager(ASceneManager&&) = delete;
			static void AddScene(AScene* scene);
			inline static void SetActiveScene(int id) { Get().m_ActiveScene = id; }
			inline static int GetActiveScene() { return Get().m_ActiveScene; }
			static AScene* GetScene(int id);
			static AScene* GetScene();

		};  

	}
}
