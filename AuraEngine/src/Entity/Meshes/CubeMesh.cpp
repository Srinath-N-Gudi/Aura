#include "CubeMesh.h"

#include "../AuraLogger.h"


namespace Aura
{
	namespace Meshes
	{

		CubeMesh::CubeMesh()
		{
			float vertices[] = {
				// pos               // normal          // texcoords
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f
			};

			unsigned int indices[] = {
				0, 1, 2, 2, 3, 0,
				4, 5, 6, 6, 7, 4,
				0, 4, 7, 7, 3, 0,
				1, 5, 6, 6, 2, 1,
				0, 1, 5, 5, 4, 0,
				3, 2, 6, 6, 7, 3
			};
			m_VBO =  new Nyx::Renderer::GL::VBO();
			m_IBO =  new Nyx::Renderer::GL::IBO();
			// Upload vertex and index data to GPU
			m_VBO->data(vertices, sizeof(vertices), sizeof(GL_FLOAT), GL_STATIC_DRAW);
			m_IBO->data(indices, sizeof(indices), sizeof(unsigned int), GL_STATIC_DRAW);
			m_VAO =  std::make_shared<Nyx::Renderer::GL::VAO>(m_VBO);
			m_VAO->setLayout({
				{0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0},         // position
				{1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (3 * sizeof(float))}, // normal
				{2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (6 * sizeof(float))}  // texcoords
				});
			m_VAO->attachIndexBuffer( m_IBO);
			AURA_CORE_INFO("CubeMesh created with {} vertices and {} indices", m_VBO->getCount(), m_IBO->getCount());
		}

		CubeMesh::~CubeMesh() {
			delete m_VBO;
			delete m_IBO;
		}

	}

}

