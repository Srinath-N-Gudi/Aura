#pragma once
#include "../Components/ATransformComponent.h"
#include "../Entity/Entity.h"
#include "../AuraAPI.h"
namespace Aura
{
	namespace Objects
	{
		class AURA_API ABaseObject : public Aura::Entity{
		public:
			ABaseObject();
			virtual ~ABaseObject();
			void setVisible(bool value);
		};

	}

	
}
