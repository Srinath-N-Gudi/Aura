#include  "MeshesManager.h"
#include "../AuraLogger.h"
namespace Aura
{
	namespace Meshes
	{
		void AMeshManager::MakeCubeMesh()
		{
			if (!Get().m_CubeMesh) {
				Get().m_CubeMesh = new CubeMesh(); // Create a new CubeMesh instance
			}
		}

		void AMeshManager::GenerateMeshes()
		{
			Get().MakeCubeMesh(); // Generate the CubeMesh
		}
		AMeshManager& AMeshManager::Get()
		{
			static AMeshManager instance;
			return instance;
		}
		CubeMesh* AMeshManager::ACubeMesh()
		{
			return Get().m_CubeMesh; // Return the CubeMesh instance
		}

		AMeshManager::AMeshManager()
			: m_CubeMesh(nullptr) // Initialize m_CubeMesh to nullptr
		{
		}
		AMeshManager::~AMeshManager() {
			AURA_CORE_INFO("[AMeshManager] : Cleaning up Meshes...");
			delete m_CubeMesh; // Clean up resources
		}
	}

}