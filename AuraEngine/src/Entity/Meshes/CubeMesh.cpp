#include "CubeMesh.h"

#include "../AuraLogger.h"


namespace Aura
{
	namespace Meshes
	{

		CubeMesh::CubeMesh()
		{
            float vertices[] = {
                // Back face (z = -0.5, normal = [0, 0, -1])
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

                // Front face (z = 0.5, normal = [0, 0, 1])
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

                // Left face (x = -0.5, normal = [-1, 0, 0])
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

                // Right face (x = 0.5, normal = [1, 0, 0])
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

                 // Bottom face (y = -0.5, normal = [0, -1, 0])
                 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
                  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
                 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

                 // Top face (y = 0.5, normal = [0, 1, 0])
                 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
                  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
            };

            unsigned int indices[] = {
                0, 1, 2, 2, 3, 0,      // Back
                4, 5, 6, 6, 7, 4,      // Front
                8, 9, 10, 10, 11, 8,   // Left
                12, 13, 14, 14, 15, 12,// Right
                16, 17, 18, 18, 19, 16,// Bottom
                20, 21, 22, 22, 23, 20 // Top
            };
			m_VBO = new Nyx::Renderer::GL::VBO();
			m_IBO =  new Nyx::Renderer::GL::IBO();
			// Upload vertex and index data to GPU
			m_VBO->data(vertices, sizeof(vertices), GL_STATIC_DRAW);
			m_IBO->data(indices, sizeof(indices), sizeof(unsigned int), GL_STATIC_DRAW);

			m_VAO =  std::make_shared<Nyx::Renderer::GL::VAO>(36);
			m_VAO->addVBO(m_VBO);
			m_VAO->setLayout({
				{0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0},         // position
				{1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (3 * sizeof(float))}, // normal
				{2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (6 * sizeof(float))}  // texcoords
				});
			m_VAO->attachIndexBuffer(m_IBO);
			AURA_CORE_INFO("CubeMesh created with {} indices", m_IBO->getCount());
		}

		CubeMesh::~CubeMesh() {
			delete m_VBO;
			delete m_IBO;
		}

	}

}

