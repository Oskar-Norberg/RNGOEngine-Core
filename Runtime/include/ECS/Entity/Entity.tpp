template<typename T, typename... Args>
// NOTE: EnTT may or may not return a reference here based on the component type. Hencewhy the decltype(auto)
auto Entity::AddComponent(Args&&... args) -> decltype(auto)
{
    return m_registry.emplace<T>(m_id, std::forward<Args>(args)...);
}

template<typename T>
T& Entity::GetComponent()
{
    return m_registry.get<T>(m_id);
}

template<typename T>
bool Entity::HasComponent() const
{
    return m_registry.any_of<T>(m_id);
}

template<typename T>
void Entity::RemoveComponent()
{
    m_registry.remove<T>(m_id);
}
