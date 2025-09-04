#include "AMaterialComponent.h"
#include <Nyx/Image/ImageLoader.h>
#include "../AuraLogger.h"
namespace Aura
{
	namespace Components
	{
		AMaterialComponent::AMaterialComponent()
		{
			m_AlbedoColor = glm::vec3(1.0f, 1.0f, 1.0f); // Default white color
			m_AlbedoMap = nullptr; // No texture by default
		}

		void AMaterialComponent::setAlbedoMap(const std::string& pathToTextureFile)
		{
			if (m_AlbedoMap) {
				delete m_AlbedoMap; // Clean up existing texture if any
			}
			Nyx::Renderer::GL::Texture2D* texture = new Nyx::Renderer::GL::Texture2D();
			Nyx::Image::Loader::LoadToTexture(*texture, pathToTextureFile);
			texture->setTextureParams();

			m_AlbedoMap = texture;
			if (!m_AlbedoMap) {
				AURA_CORE_ERROR("[AMaterialComponent] : Failed to load texture from path: {}", pathToTextureFile);
				m_AlbedoMap = nullptr;
			}
			else {
				AURA_CORE_INFO("[AMaterialComponent] : Successfully loaded texture from path: {}", pathToTextureFile);
			}
		}

	}
}
