#include "ARenderer.h"
#include "../AuraLogger.h"
#include "../Scene/ASceneManager.h"
#include "../AShader/AShaderManager.h"
#include "../Entity/Components/ATransformComponent.h"
#include "../Entity/Components/AMaterialComponent.h"
#include <memory>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

namespace Aura {
	ARenderer::~ARenderer() {
		delete m_Renderer;
	}
	void ARenderer::Idraw() {
		auto* basicShader = AShaderManager::Get().Get("basic");

		auto* scene = Scene::ASceneManager::GetScene();
		if (!scene) {
			AURA_CORE_ERROR("[ARenderer] : No active scene found! Please add atleast a single scene to render");
			return;
		}
			for (const auto& pair : scene->getEntityManager().m_Entities) {

			Entity* entity = pair.second;
			// For every entity

			if (!entity) {
				AURA_CORE_WARN("[ARenderer] : Encountered null entity pointer, skipping...");
				continue;
			}
			



			if (!entity->isAlive())
				continue;


			// End Every entity
			if (entity->isVisible()) {

			
				// dynamic cast to AComponent
				auto* component = entity->getComponent<Components::ATransformComponent>();
				auto* material = entity->getComponent<Components::AMaterialComponent>();
				basicShader->bind();
				if (material) {
					// Check if material has texture 
					if (material->getAlbedoMap())
					{
						material->getAlbedoMap()->ActivateTextureAtSlot(0);
						basicShader->setUniform1i("useTexture", 1); 
						basicShader->setUniform1i("u_Texture", 0);
					}
					else // material map is not set fallback to defaulut color
					{
						glm::vec3 color = material->getAlbedoColor();
						basicShader->setUniform1i("useTexture", 0);
						basicShader->setUniform3f("albedoColor", color.r, color.g, color.b);

					}
				}
				//glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
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
	}
}