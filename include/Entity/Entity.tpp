#pragma once

template<typename T, typename... Args>
void Entity::AddComponent(Args&& ...args)
{
    m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
}

template<typename T>
T& Entity::GetComponent()
{
    return m_registry.get<T>(m_entity);
}

template<typename T>
bool Entity::TryGetComponent(T& outComponent)
{
    if (m_registry.all_of<T>(m_entity))
    {
        outComponent = m_registry.get<T>(m_entity);
        return true;
    }

    return false;
}