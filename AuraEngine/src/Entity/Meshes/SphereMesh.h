#pragma once 
#include "../AuraAPI.h"  
#include <memory>
#include <Nyx/Renderer/GL/VBO.h>
#include <Nyx/Renderer/GL/IBO.h>
#include <Nyx/Renderer/GL/VAO.h>

namespace Aura
{
	namespace Meshes
	{
		struct SphereMeshContainer {
			std::vector<float> vertices; // interleaved pos, normal, uv
			std::vector<unsigned int> indices;
		};
		class SphereMesh {
		public:
			SphereMesh();
			~SphereMesh();

			void getVAO(std::shared_ptr<Nyx::Renderer::GL::VAO>& vao)
			{
				vao = m_VAO;
			}
		private:
			Nyx::Renderer::GL::VBO* m_VBO;
			std::shared_ptr<Nyx::Renderer::GL::VAO> m_VAO;
			Nyx::Renderer::GL::IBO* m_IBO;
			SphereMeshContainer generateSphereMesh(float radius, int segments);

		};
	};

}

