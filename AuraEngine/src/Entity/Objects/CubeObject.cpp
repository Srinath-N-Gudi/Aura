#include "../AEntityManager.h"
#include "CubeObject.h"


namespace Aura
{
	namespace Objects
	{
		CubeObject::CubeObject(const std::string& name, const std::string& tag)
		{
			setName(name);
			setTag(tag);
			m_IsDrawable = true; // Set the object as drawable
			AEntityManager::addEntity(this); // Add this entity to the EntityManager
		}
	
		CubeObject::~CubeObject() {
			AEntityManager::removeEntity(getID()); // Remove this entity from the EntityManager
		}
	}

}