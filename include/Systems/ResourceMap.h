//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <any>
#include <typeindex>
#include <unordered_map>

namespace RNGOEngine::Systems::Resources
{
    class ResourceMap
    {
    public:
        template<typename T>
        void AddResource(T&& resource, bool override = false)
        {
            if (!override)
            {
                const auto it = m_resources.find(typeid(T));
                if (it != m_resources.end())
                {
                    return;
                }
            }

            m_resources[typeid(T)] = std::forward<T>(resource);
        }

        template<typename T>
        bool TryGetResource(T& outResource) const
        {
            const auto it = m_resources.find(typeid(T));
            if (it != m_resources.end())
            {
                outResource = std::any_cast<T>(it->second);
                return true;
            }

            return false;
        }

        void Clear()
        {
            m_resources.clear();
        }

    private:
        std::unordered_map<std::type_index, std::any> m_resources;
    };
}