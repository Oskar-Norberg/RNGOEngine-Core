#pragma once

template <InheritsISystem<> T>
void SystemScheduler::RegisterSystem(const std::string& name, const std::vector<std::string>& before,
    const std::vector<std::string>& after)
{
    // Create Registered System (on the heap unfortunately).
    std::unique_ptr<RegisteredSystem> system = std::make_unique<RegisteredSystem>(std::make_unique<T>(), name, before, after);

    // Transfer ownership to the list.
    m_systems.emplace_back(std::move(system));

    // Add iterator to the uninitialized systems queue.
    const auto it = std::prev(m_systems.end());
    m_uninitializedSystems.push(it);
}

template <InheritsISystem<> T>
void SystemScheduler::UnregisterSystem()
{
    m_systems.emplace_back(std::make_unique<T>());
}
