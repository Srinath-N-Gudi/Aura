#pragma once
#include "../AuraAPI.h"
#include <glm/glm.hpp>
#include "ABaseComponent.h"
namespace Aura
{
	class ARenderer;
	namespace Components
	{
		class AURA_API ATransformComponent : public Aura::ABaseComponent
		{
		public:
			ATransformComponent();
			~ATransformComponent() = default; 


			glm::vec3 position; // Position in 3D space
			glm::vec3 rotation; // Rotation in degrees (pitch, yaw, roll)
			glm::vec3 scale;    // Scale factors along each axis
			friend class ARenderer;
		private:
			glm::mat4 getModelMatrix() const; // Compute and return the model matrix
		};
	
	}

}
