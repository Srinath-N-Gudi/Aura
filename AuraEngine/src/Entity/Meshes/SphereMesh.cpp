#include "SphereMesh.h"

#include "../AuraLogger.h"

#define M_PI 3.14159265358979323846

namespace Aura
{
    namespace Meshes
    {

        SphereMesh::SphereMesh()
        {
            SphereMeshContainer sphere = generateSphereMesh(1.0f, 32);
            m_VBO = new Nyx::Renderer::GL::VBO();
            m_IBO = new Nyx::Renderer::GL::IBO();
            // Upload vertex and index data to GPU
            m_VBO->data(sphere.vertices.data(), sphere.vertices.size()*sizeof(float), GL_STATIC_DRAW);
            m_IBO->data(sphere.indices.data(), sphere.indices.size()*sizeof(float), sizeof(unsigned int), GL_STATIC_DRAW);

            m_VAO = std::make_shared<Nyx::Renderer::GL::VAO>(sphere.indices.size());
            m_VAO->addVBO(m_VBO);
            m_VAO->setLayout({
                {0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0 }, // Position attribute
                { 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float) }, // Normal attribute
                { 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 6 * sizeof(float) } // Texture coordinate 
        });
            m_VAO->attachIndexBuffer(m_IBO);
            AURA_CORE_INFO("SphereMesh created with {} indices", sphere.indices.size());
        }
        SphereMeshContainer SphereMesh::generateSphereMesh(float radius, int segments) {
            SphereMeshContainer mesh;

            // --- Generate vertices ---
            for (int i = 0; i <= segments; ++i) {
                float theta = i * M_PI / segments;
                float sinTheta = sin(theta);
                float cosTheta = cos(theta);

                for (int j = 0; j <= segments; ++j) {
                    float phi = j * 2.0f * M_PI / segments;
                    float sinPhi = sin(phi);
                    float cosPhi = cos(phi);

                    // Position
                    float x = radius * sinTheta * cosPhi;
                    float y = radius * cosTheta;
                    float z = radius * sinTheta * sinPhi;

                    // Normal
                    float nx = sinTheta * cosPhi;
                    float ny = cosTheta;
                    float nz = sinTheta * sinPhi;

                    // UV
                    float u = (float)j / segments;
                    float v = (float)i / segments;

                    mesh.vertices.insert(mesh.vertices.end(), { x, y, z, nx, ny, nz, u, v });
                }
            }

            // --- Generate indices ---
            for (int i = 0; i < segments; ++i) {
                for (int j = 0; j < segments; ++j) {
                    int a = i * (segments + 1) + j;
                    int b = (i + 1) * (segments + 1) + j;
                    int c = a + 1;
                    int d = b + 1;

                    // Triangle 1
                    mesh.indices.push_back(a);
                    mesh.indices.push_back(b);
                    mesh.indices.push_back(c);

                    // Triangle 2
                    mesh.indices.push_back(c);
                    mesh.indices.push_back(b);
                    mesh.indices.push_back(d);
                }
            }

            return mesh;
        }
                 SphereMesh::~SphereMesh() {


            delete m_VBO;
            delete m_IBO;
		 }

	}
}


