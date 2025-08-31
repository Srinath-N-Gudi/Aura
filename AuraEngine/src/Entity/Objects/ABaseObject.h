#pragma once
#include "../Components/ATransformComponent.h"
#include "../Entity/Entity.h"
#include "../AuraAPI.h"
namespace Aura
{
	namespace Objects
	{
		class AURA_API ABaseObject : public Aura::Entity, public Aura::Components::ATransformComponent
		{
		public:
			ABaseObject() = default;
			virtual ~ABaseObject();
			void setVisible(bool value);
		};

	}

	
}
