#pragma once
#include "../AuraAPI.h"
#include "../AuraLogger.h"
#include "ABaseComponent.h"

#include <unordered_map>
#include <typeindex>

namespace Aura
{
    class AURA_API AComponentHelper
    {
    private:
        std::unordered_map<std::type_index, ABaseComponent*> m_Components;

    public:
        AComponentHelper() = default;
        virtual ~AComponentHelper();

        template <typename ComponentType>
        ComponentType* getComponent();

        template <typename ComponentType>
        ComponentType* addComponent();

        template <typename ComponentType>
        void removeComponent();
    };
}
