#include "../AEntityManager.h"
#include "ACubeObject.h"


namespace Aura
{
	namespace Objects
	{
		ACubeObject::ACubeObject(const std::string& name, const std::string& tag)
		{
			setName(name);
			setTag(tag);
			m_IsDrawable = true; // Set the object as drawable
			AEntityManager::addEntity(this); // Add this entity to the EntityManager
		}


	}

}