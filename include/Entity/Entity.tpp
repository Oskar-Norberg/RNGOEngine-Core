#pragma once

template<typename T>
void Entity::AddComponent(const T& component)
{
    m_registry.emplace<T>(m_entity, component);
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