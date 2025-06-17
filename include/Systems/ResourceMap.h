//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <any>
#include <typeindex>
#include <unordered_map>

class ResourceMap
{
public:
    // Make a try add resource and a set resource function. User might not want to override existing resources.
    template<typename T>
    void AddResource(const T& resource)
    {
        m_resources[typeid(T)] = resource;
    }

    template<typename T>
    bool TryGetResource(T& outResource) const
    {
        auto it = m_resources.find(typeid(T));
        if (it != m_resources.end())
        {
            outResource = std::any_cast<T>(it->second);
            return true;
        }

        return false;
    }

private:
    std::unordered_map<std::type_index, std::any> m_resources;
};
