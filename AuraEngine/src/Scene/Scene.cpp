#include "Scene.h"
#include "ASceneManager.h"
#include "../Entity/Objects/ACubeObject.h"

namespace Aura
{
	namespace Scene
	{
		AScene::AScene()
		{
			m_ID = -1; // Will be set by SceneManager when added
			ASceneManager::AddScene(this);
		}
		AScene::~AScene()
		{
			// Cleanup if necessary
			m_EntityManager.clearEntities();
		}

		template <typename ObjectType>
		ObjectType* AScene::AddObject(const std::string& name, std::string tag)
		{
			ObjectType* newObject = new ObjectType(name, tag);
			m_EntityManager.addEntity(newObject);
			return newObject; // Ensure the object is returned
		}

		// Explicit instantiation for ACubeObject
		template AURA_API Objects::ACubeObject* AScene::AddObject<Objects::ACubeObject>(const std::string& name, std::string tag);
	}
}
