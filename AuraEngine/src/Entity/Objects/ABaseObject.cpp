#include "ABaseObject.h"
#include "../AuraLogger.h"

namespace Aura
{
	namespace Objects
	{
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