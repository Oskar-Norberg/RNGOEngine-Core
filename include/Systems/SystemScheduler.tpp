template<typename TSystemContext>
void SystemScheduler<TSystemContext>::Terminate(Core::World& world, TSystemContext& context)
{
    for (const auto& system : m_systems)
    {
        system->Exit(world, context);
    }
}

template<typename TSystemContext>
void SystemScheduler<TSystemContext>::Update(Core::World& world, TSystemContext& context)
{
    InitializeSystems(world, context);
    
    for (const auto& system : m_systems)
    {
        system->Update(world, context);
    }
}

template<typename TSystemContext>
template<Concepts::DerivedFrom<ISystem<TSystemContext>> T, typename... Args>
void SystemScheduler<TSystemContext>::RegisterSystem(Args&&... args)
{
    m_systems.emplace_back(
        ScheduledSystem<TSystemContext>{std::make_unique<T>(std::forward<Args>(args)...)});
}

template<typename TSystemContext>
void SystemScheduler<TSystemContext>::InitializeSystems(Core::World& world, TSystemContext& context)
{
    for (const auto& system : m_systems)
    {
        if (!system.initialized)
        {
            system->Initialize(world, context);
        }
    }
}