#include "../AEntityManager.h"
#include "ASphereObject.h"


namespace Aura
{
	namespace Objects
	{
		ASphereObject::ASphereObject(const std::string& name, const std::string& tag)
		{
			setName(name);
			setTag(tag);
			m_IsDrawable = true; // Set the object as drawable
		}


	}

}