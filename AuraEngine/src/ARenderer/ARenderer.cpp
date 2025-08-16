#include "ARenderer.h"
#include "../Entity/EntityManager.h"
#include "../Globals.h"
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
	void ARenderer::draw() {
		std::vector<std::shared_ptr<Nyx::Renderer::GL::VAO>> vaos;
		vaos.reserve(AEntityManager.getEntityCount());
		for (const auto& pair : AEntityManager.m_Entities) {
			Entity* entity = pair.second;
			if (entity->IsDrawable()) {
					vaos.push_back(entity->getVAO());
			}
		}
		m_Renderer->draw(vaos.data(), vaos.size());
	}
}

Aura::ARenderer Renderer; // Global instance of ARenderer