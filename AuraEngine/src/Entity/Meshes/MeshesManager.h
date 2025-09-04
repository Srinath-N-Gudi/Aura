#pragma once
#include "CubeMesh.h"  
#include "SphereMesh.h"
#include "../../AuraAPI.h"  

namespace Aura
{
	namespace Meshes
	{
		class AURA_API AMeshManager
		{
		private:
			AMeshManager();
			AMeshManager(const AMeshManager&) = delete; // Prevent copying  

			CubeMesh* m_CubeMesh; // Initialize pointer to nullptr  
			SphereMesh* m_SphereMesh;

			void MakeCubeMesh();
			void MakeSphereMesh();
		public:
			static AMeshManager& Get();
			static void GenerateMeshes();
			static CubeMesh* ACubeMesh();
			static SphereMesh* ASphereMesh();
			~AMeshManager();
		};
	}
}