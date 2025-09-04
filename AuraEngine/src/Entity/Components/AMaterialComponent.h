#pragma once
#include "../AuraAPI.h"
#include "ABaseComponent.h"
#include <Nyx/Renderer/GL/Texture2D.h>
#include <glm/glm.hpp>

namespace Aura
{
	namespace Components
	{

		class AURA_API AMaterialComponent : public Aura::ABaseComponent
		{

		private: 
			glm::vec3 m_AlbedoColor;
			Nyx::Renderer::GL::Texture2D* m_AlbedoMap;
		public:
			AMaterialComponent();
			~AMaterialComponent() = default;
			inline glm::vec3 getAlbedoColor() const { return m_AlbedoColor; }
			inline Nyx::Renderer::GL::Texture2D* getAlbedoMap() const { return m_AlbedoMap; }

			inline void setAlbedoColor(const glm::vec3& color) { m_AlbedoColor = color; }
			void setAlbedoMap(const std::string& pathToTextureFile);
		};

	}


}
