//
// Created by Oskar.Norberg on 2025-08-06.
//

#pragma once

#include "ResourceMap.h"

namespace RNGOEngine::Systems::Resources
{
    class ResourceMapper
    {
    public:
        template<typename T>
        bool TryGetTransientResource(T& outResource) const
        {
            return m_transientResources.TryGetResource(outResource);
        }

        template<typename T>
        void AddTransientResource(T&& resource, bool override = false)
        {
            m_transientResources.AddResource(std::forward<T>(resource), override);
        }

        template<typename T>
        bool TryGetPersistentResource(T& outResource) const
        {
            return m_persistentResources.TryGetResource(outResource);
        }

        template<typename T>
        void AddPersistentResource(T&& resource, bool override = false)
        {
            m_persistentResources.AddResource(std::forward<T>(resource), override);
        }

        void ClearTransientResources()
        {
            m_transientResources.Clear();
        }

    private:
        ResourceMap m_transientResources;
        ResourceMap m_persistentResources;
    };
}