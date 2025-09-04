#pragma once
#include "../AuraAPI.h"
#include "ABaseComponent.h"
#include <Nyx/Renderer/GL/Texture2D.h>
#include <glm/glm.hpp>
#include <memory>
namespace Aura
{
	namespace Components
	{
		struct AURA_API AMaterial {
			glm::vec3 albedoColor = glm::vec3(1.0f);
			std::shared_ptr<Nyx::Renderer::GL::Texture2D> albedoMap;
			void setAlbedoMap(const std::string& pathToTextureFile);
			static AMaterial* MakeMaterial() { return new AMaterial(); }
		};
		class AURA_API AMaterialComponent : public Aura::ABaseComponent
		{

		private:
			AMaterial m_Material;
		public:
			AMaterialComponent();
			~AMaterialComponent() = default;
			inline glm::vec3 getAlbedoColor() const { return m_Material.albedoColor; }
			inline std::shared_ptr<Nyx::Renderer::GL::Texture2D> getAlbedoMap() const { return m_Material.albedoMap; }

			inline void setAlbedoColor(const glm::vec3& color) { m_Material.albedoColor = color; }
			void setAlbedoMap(const std::string& pathToTextureFile);
			void setAlbedoMap(std::shared_ptr<Nyx::Renderer::GL::Texture2D>& texture) { m_Material.albedoMap = texture; }
			void setMaterial(const AMaterial& material) { m_Material = material; }
			inline AMaterial getMaterial() const { return m_Material; }
		};
	}
}
