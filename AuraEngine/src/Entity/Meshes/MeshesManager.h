#include "CubeMesh.h"  
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

			void MakeCubeMesh();
		public:
			static AMeshManager& Get();
			static void GenerateMeshes();
			static CubeMesh* ACubeMesh();
			~AMeshManager();
		};
	}
}