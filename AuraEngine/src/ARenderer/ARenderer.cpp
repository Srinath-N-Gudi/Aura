#include "ARenderer.h"
#include "../Entity/AEntityManager.h"
#include "../AuraLogger.h"
#include "../AShader/AShaderManager.h"
#include "../Entity/Components/ATransformComponent.h"
#include <memory>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

namespace Aura {
	ARenderer::ARenderer()
	{
	}
	ARenderer::~ARenderer() {
		delete m_Renderer;
	}
	void ARenderer::Idraw() {
		auto* basicShader = AShaderManager::Get().Get("basic");
		for (const auto& pair : AEntityManager::Entities()) {
			Entity* entity = pair.second;
			// For every entity

			if (!entity) {
				AURA_CORE_WARN("[ARenderer] : Encountered null entity pointer, skipping...");
				continue;
			}
			



			if (!entity->IsAlive())
				continue;


			// End Every entity
			if (entity->IsVisible()) {

			
				// dynamic cast to AComponent
				auto component = dynamic_cast<Components::ATransformComponent*>(entity);
				basicShader->bind();
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
				basicShader->setUniformMat4fv("AProjection", glm::value_ptr(defaultProjection));
				basicShader->setUniformMat4fv("AModel", glm::value_ptr(component->getModelMatrix())); 

				auto vao = entity->getVAO();
                    vao->bind();
                    if (vao->hasIBO()) {
                        glDrawElements(GL_TRIANGLES, vao->getTotalVertices(), GL_UNSIGNED_INT, nullptr);
                    }
                    else {
                        glDrawArrays(GL_TRIANGLES, 0, (vao->getTotalVertices())); // Assuming all the Layouts are filled uniformly
                    }
			}
		}
	}
	void ARenderer::draw() {
		Get().Idraw(); // Call the internal draw method
	}
	ARenderer& ARenderer::Get() {
		static ARenderer instance; // Create a static instance of ARenderer
		return instance; // Return the singleton instance
 } } 