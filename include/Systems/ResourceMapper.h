//
// Created by Oskar.Norberg on 2025-08-06.
//

#pragma once

#include "ResourceMap.h"


class ResourceMapper
{
public:
    template<typename T>
    bool TryGetTransientResource(T& outResource) const
    {
        return m_transientResources.TryGetResource(outResource);
    }

    template<typename T>
    void AddTransientResource(const T resource, bool override = false)
    {
        m_transientResources.AddResource(resource, override);
    }

    template<typename T>
    bool TryGetPersistentResource(T& outResource) const
    {
        return m_persistentResources.TryGetResource(outResource);
    }

    template<typename T>
    void AddPersistentResource(const T resource, bool override = false)
    {
        m_persistentResources.AddResource(resource, override);
    }

    void ClearTransientResources()
    {
        m_transientResources.Clear();
    }

private:
    ResourceMap m_transientResources;
    ResourceMap m_persistentResources;
};
