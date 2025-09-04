#include "AMaterialComponent.h"
#include <Nyx/Image/ImageLoader.h>
#include "../AuraLogger.h"
namespace Aura
{
	namespace Components
	{
		AMaterialComponent::AMaterialComponent()
		{
			m_Material = { glm::vec3(1.0f, 1.0f, 1.0f) };
		}

		void AMaterialComponent::setAlbedoMap(const std::string& pathToTextureFile)
		{
			m_Material.setAlbedoMap(pathToTextureFile);
		}
		void AMaterial::setAlbedoMap(const std::string& pathToTextureFile)
		{
			std::shared_ptr<Nyx::Renderer::GL::Texture2D> texture = std::make_shared<Nyx::Renderer::GL::Texture2D>();
			Nyx::Image::Loader::LoadToTexture(*texture, pathToTextureFile);
			texture->setTextureParams();

			albedoMap = texture;
			if (!texture) {
				AURA_CORE_ERROR("[AMaterialComponent] : Failed to load texture from path: {}", pathToTextureFile);
			}
			else {
				AURA_CORE_INFO("[AMaterialComponent] : Successfully loaded texture from path: {}", pathToTextureFile);
			}
		
		}
	}
}
