#include "ARenderer.h"
#include "../Entity/AEntityManager.h"
#include "../AuraLogger.h"
#include <memory>
#include <vector>

namespace Aura {
	ARenderer::ARenderer()
	{
		m_Renderer = new Nyx::Renderer::GL::Renderer(GL_TRIANGLES); // Initialize the renderer with a draw mode
	}
	ARenderer::~ARenderer() {
		delete m_Renderer;
	}
	void ARenderer::Idraw() {
		std::vector<std::shared_ptr<Nyx::Renderer::GL::VAO>> vaos;
		vaos.reserve(AEntityManager::getEntityCount());
		for (const auto& pair : AEntityManager::Entities()) {
			Entity* entity = pair.second;
			if (entity->IsDrawable()) {
					vaos.push_back(entity->getVAO());
			}
		}
		AURA_CORE_INFO("Drawing {} entities", vaos.size());
		m_Renderer->draw(vaos.data(), vaos.size());
	}
	void ARenderer::draw() {
		Get().Idraw(); // Call the internal draw method
	}
	ARenderer& ARenderer::Get() {
		static ARenderer instance; // Create a static instance of ARenderer
		return instance; // Return the singleton instance
 } } 