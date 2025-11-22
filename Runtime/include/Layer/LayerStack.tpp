template<Concepts::DerivedFrom<Layer> T, typename... Args>
T& LayerStack::PushLayer(Args&&... args)
{
    m_layers.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    m_layers.back()->OnAttach();
    return static_cast<T&>(*m_layers.back());
}

template<Concepts::DerivedFrom<Layer> T>
bool LayerStack::PopLayer()
{
    for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
    {
        if (dynamic_cast<T*>(it->get()) != nullptr)
        {
            (*it)->OnDetach();
            m_layers.erase(it);
            return true;
        }
    }

    return false;
}