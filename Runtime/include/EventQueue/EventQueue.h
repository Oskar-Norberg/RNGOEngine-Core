//
// Created by ringo on 2025-08-06.
//

#pragma once

#include <vector>
#include <any>
#include <typeindex>
#include <unordered_map>

#include "Profiling/Profiling.h"

namespace RNGOEngine::Events
{
    class EventQueue
    {
    public:
        template<typename T>
        void PushEventMultiple(std::vector<T>& event)
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("EventQueue::PushEventBulk");

            const auto typeIndex = std::type_index(typeid(T));
            auto& eventList = m_currentEvents[typeIndex];
            eventList.reserve(eventList.size() + event.size());
            
            for (auto&& e : event)
            {
                eventList.emplace_back(std::move(e));
            }
        }

        template<typename T>
        void PushEvent(T&& event)
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("EventQueue::PushEvent");

            const auto typeIndex = std::type_index(typeid(T));
            m_currentEvents[typeIndex].emplace_back(std::move(event));
        }

        template<typename T, typename... Args>
        void EmplaceEvent(Args&&... args)
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("EventQueue::EmplaceEvent");

            const auto typeIndex = std::type_index(typeid(T));
            m_currentEvents[typeIndex].emplace_back(T(std::forward<Args>(args)...));
        }

        template<typename T>
        // TODO: Really expensive does a full copy of all events.
        // Implement custom iterator that can automatically cast the events to the correct type.
        std::vector<T> GetEvents() const
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("EventQueue::GetEvents");

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
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("EventQueue::GetEvents");

            m_currentEvents.clear();
        }

    private:
        std::unordered_map<std::type_index, std::vector<std::any>> m_currentEvents;
    };
}