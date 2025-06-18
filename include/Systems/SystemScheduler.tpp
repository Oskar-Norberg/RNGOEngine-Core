#pragma once

template <InheritsISystem<> T>
void SystemScheduler::RegisterSystem(const std::string& name, const std::vector<std::string>& before,
    const std::vector<std::string>& after)
{
    AddSystem(std::make_unique<T>());
}

template <InheritsISystem<> T>
void SystemScheduler::UnregisterSystem()
{
    m_systems.emplace_back(std::make_unique<T>());
}
