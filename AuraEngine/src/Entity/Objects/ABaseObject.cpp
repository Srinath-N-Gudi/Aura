#include "ABaseObject.h"  
#include "../Components/AComponentHelper.h"
#include "../Components/ATransformComponent.h"  
#include "../AuraLogger.h"  

namespace Aura  
{  
namespace Objects  
{  
	ABaseObject::ABaseObject()  
	{  
		// Ensure the addComponent method is accessible by making it public in the base class  
		this->addComponent<Aura::Components::ATransformComponent>();  
	}  
	void ABaseObject::setVisible(bool value)  
	{  
		if (!m_IsAlive)  
			AURA_CORE_WARN("[ABaseObject] : Trying to set visibility of a dead object (ID: {})", m_ID);  
		m_IsDrawable = value;  
	}  
	ABaseObject::~ABaseObject()  
	{  
		AURA_CORE_INFO("[ABaseObject] : Destroying Object ID: {}", m_ID);  
		m_IsAlive = false; // Mark the object as dead  
	}  

}  

}