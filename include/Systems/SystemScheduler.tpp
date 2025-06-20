#pragma once
#include "Utilities/Containers/Graphs/DirectedGraph.h"

template <InheritsISystem<> T>
void SystemScheduler::RegisterSystem(const std::string& name, const std::vector<std::string>& before,
    const std::vector<std::string>& after)
{
    // Create new registered system.
    m_systems.emplace_back(std::make_unique<RegisteredSystem>(std::make_unique<T>(), name, before, after));

    // Name -> RegisteredSystem* Lookup table.
    std::unordered_map<std::string, RegisteredSystem*> systemMap;
    for (auto& registeredSystem : m_systems)
    {
        systemMap[registeredSystem->name] = registeredSystem.get();
    }

    // Construct digraph of systems.
    DirectedGraph<RegisteredSystem> graph;
    // TODO: This is slow, build graph through constructor, passing vectors of vertices and edges.
    for (const auto& system : m_systems)
    {
        graph.AddVertex(system.get());

        if (!system->before.empty())
        {
            for (const auto& string : system->before)
            {
                graph.AddEdge(systemMap[string], system.get());
            }
        }

        if (!system->after.empty())
        {
            for (const auto& string : system->after)
            {
                graph.AddEdge(system.get(), systemMap[string]);
            }
        }
    }

    std::vector<RegisteredSystem*> sortedSystems = graph.TopologicalSort();

    // Sort the owning list of systems based on the topological sort.
    // Sort the owning list of systems based on the topological sort.
    std::unordered_map<RegisteredSystem*, size_t> orderMap;
    for (size_t i = 0; i < sortedSystems.size(); ++i)
    {
        orderMap[sortedSystems[i]] = i;
    }

    std::sort(m_systems.begin(), m_systems.end(),
        [&orderMap](const std::unique_ptr<RegisteredSystem>& a, const std::unique_ptr<RegisteredSystem>& b) {
            return orderMap[a.get()] < orderMap[b.get()];
        });
}

template <InheritsISystem<> T>
void SystemScheduler::UnregisterSystem()
{
    m_systems.emplace_back(std::make_unique<T>());
}
