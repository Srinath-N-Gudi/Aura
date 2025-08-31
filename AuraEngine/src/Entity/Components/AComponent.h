#pragma once
#include "../AuraAPI.h"

namespace Aura
{
	class AURA_API AComponent
	{
	public:
		virtual ~AComponent() = default;
	
		// All things will be put here after the AuraEditor is done.
		// Every ABaseObject that has a AComponent will be able to show the component properties in the editor.
	
	};

}
