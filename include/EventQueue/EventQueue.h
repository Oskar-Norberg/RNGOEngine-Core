//
// Created by ringo on 2025-08-06.
//

#pragma once

#include <any>
#include <queue>
#include <typeindex>
#include <unordered_map>

#include "Concepts/Concepts.h"
#include "IEvent.h"

class EventQueue
{
public:
    template<DerivedFrom<IEvent> T, typename... Args>
    void EmplaceEvent(Args&&... args)
    {
        const auto typeIndex = std::type_index(typeid(T));
        auto it = m_currentEvents.find(typeIndex);

        if (it == m_currentEvents.end())
        {
            m_currentEvents[typeIndex] = {};
            it = m_currentEvents.find(typeIndex);
        }

        it->second.emplace_back(T(std::forward<Args>(args)...));
    }

    template<DerivedFrom<IEvent> T>
    // TODO: Really expensive does a full copy of all events.
    // Implement custom iterator that can automatically cast the events to the correct type.
    std::vector<T> GetEvents() const
    {
        std::vector<T> events;
        const auto typeIndex = std::type_index(typeid(T));

        const auto it = m_currentEvents.find(typeIndex);
        if (it != m_currentEvents.end())
        {
            events.reserve(it->second.size());
            for (const auto& eventPtr : it->second)
            {
                events.push_back(std::any_cast<T>(eventPtr));
            }
        }

        return events;
    }

    void Clear()
    {
        m_currentEvents.clear();
    }

private:
    std::unordered_map<std::type_index, std::vector<std::any>> m_currentEvents;
};
