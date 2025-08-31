#include "ATransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>


namespace Aura
{
	namespace Components
	{
		ATransformComponent::ATransformComponent()
			: position(0.0f, 0.0f, 0.0f),
			rotation(0.0f, 0.0f, 0.0f),
			scale(1.0f, 1.0f, 1.0f)
		{
		}

		glm::mat4 ATransformComponent::getModelMatrix() const
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, scale);
			return model;
		}
	
	
	}


}