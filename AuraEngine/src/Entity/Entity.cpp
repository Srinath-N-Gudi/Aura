#include "Entity.h"
#include "../AuraLogger.h"

namespace Aura
{
	void Entity::setVisible(bool value)
	{
		if (!m_IsAlive)
			AURA_CORE_WARN("[Entity] : Trying to set visibility of a dead entity (ID: {})", m_ID);
		m_IsDrawable = value;
	}

}