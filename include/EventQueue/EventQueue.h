//
// Created by ringo on 2025-08-06.
//

#pragma once

#include <any>
#include <queue>
#include <typeindex>
#include <unordered_map>

class EventQueue
{
public:
    template<typename T>
    void PushEvent(T&& event)
    {
        std::type_index typeIndex = typeid(T);
        auto it = m_currentEvents.find(typeIndex);

        if (it == m_currentEvents.end())
        {
            m_currentEvents[typeIndex] = std::queue<std::any>();
        }

        m_currentEvents[typeIndex].push(std::forward<T>(event));
    }

    template<typename T>
    const std::queue<std::any>& GetEvents() const
    {
        return m_currentEvents[typeid(T)];
    }

private:
    std::unordered_map<std::type_index, std::queue<std::any>> m_currentEvents;
};
