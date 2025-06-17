#pragma once

template<InheritsISystem<> T>
void SystemScheduler::RegisterSystem()
{
    AddSystem(std::make_unique<T>());
}

template <InheritsISystem<> T>
void SystemScheduler::UnregisterSystem()
{
    m_systems.emplace_back(std::make_unique<T>());
}
