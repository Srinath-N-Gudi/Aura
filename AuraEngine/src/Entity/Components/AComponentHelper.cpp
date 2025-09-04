#include "AComponentHelper.h"
#include "ATransformComponent.h"
#include "AMaterialComponent.h"
#include <algorithm>

#define INSTATIATE_COMPONENT(Type)\
template AURA_API Type* AComponentHelper::getComponent<Type>();\
template AURA_API Type* AComponentHelper::addComponent<Type>();\
template AURA_API void AComponentHelper::removeComponent<Type>()\



namespace Aura
{
    AComponentHelper::~AComponentHelper()
    {
        for (auto& [type, comp] : m_Components)
        {
            delete comp;
        }
        AURA_CORE_INFO("[Entity] : Clearing Components");
        m_Components.clear();
    }

    template <typename ComponentType>
    ComponentType* AComponentHelper::getComponent()
    {
        auto type = std::type_index(typeid(ComponentType));
        auto it = m_Components.find(type);
        if (it == m_Components.end())
        {
            //AURA_CORE_ERROR("[AComponentHelper] : Component not found!");
            return nullptr;
        }
        return static_cast<ComponentType*>(it->second);
    }

    template <typename ComponentType>
    ComponentType* AComponentHelper::addComponent()
    {
        auto type = std::type_index(typeid(ComponentType));
        if (m_Components.find(type) == m_Components.end())
        {
            m_Components[type] = new ComponentType();
			return static_cast<ComponentType*>(m_Components[type]);
        }
        else
        {
            AURA_CORE_WARN("[AComponentHelper] : Component already exists!");
            return nullptr;
        }
    }

    template <typename ComponentType>
    void AComponentHelper::removeComponent()
    {
        auto type = std::type_index(typeid(ComponentType));
        auto it = m_Components.find(type);
        if (it != m_Components.end())
        {
            delete it->second;
            m_Components.erase(it);
        }
    }

    INSTATIATE_COMPONENT(Components::ATransformComponent);
    INSTATIATE_COMPONENT(Components::AMaterialComponent);
}
