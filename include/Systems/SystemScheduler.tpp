#pragma once

#include <functional>
#include <stack>
#include <unordered_map>
#include <unordered_set>

template <InheritsISystem<> T>
void SystemScheduler::RegisterSystem(const std::string& name, const std::vector<std::string>& before,
    const std::vector<std::string>& after)
{
    // Create Registered System (on the heap unfortunately).
    std::unique_ptr<RegisteredSystem> newSystem = std::make_unique<RegisteredSystem>(std::make_unique<T>(), name, before, after);

    // Move existing systems for reordering.
    std::vector<std::unique_ptr<RegisteredSystem>> vertices;
    vertices.reserve(m_systems.size() + 1);
    for (auto& registered_system : m_systems)
    {
        vertices.push_back(std::move(registered_system));
    }
    vertices.emplace_back(std::move(newSystem));

    // Initialize edge lists.
    std::pmr::unordered_map<const RegisteredSystem*, std::vector<const RegisteredSystem*>> edges;
    for (const auto& registered_system : vertices)
    {
        edges[registered_system.get()] = std::vector<const RegisteredSystem*>();
    }

    // Lookup table for system names.
    std::unordered_map<std::string_view, RegisteredSystem*> name_to_system;
    for (const auto& sys : vertices) {
        name_to_system[sys->name] = sys.get();
    }

    // Fill edges based on before and after dependencies.
    for (const auto& registered_system : vertices)
    {
        // Before dependencies.
        if (!registered_system->before.empty())
        {
            for (const auto& before_system : registered_system->before)
            {
                const auto it = name_to_system.find(before_system);
                if (it == name_to_system.end())
                {
                    // Before dependency is not registered. Log a warning? Haven't built a logging system yet...
                    continue;
                }

                edges[registered_system.get()].push_back(it->second);
            }
        }

        // After dependencies.
        if (!registered_system->after.empty())
        {
            for (const auto& after_system : registered_system->after)
            {
                const auto it = name_to_system.find(after_system);

                if (it == name_to_system.end())
                {
                    // After dependency is not registered. Log a warning?
                    continue;
                }

                edges[it->second].push_back(registered_system.get());
            }
        }
    }

    // DFS Topological sort
    std::pmr::unordered_set<RegisteredSystem*> visited;
    std::pmr::unordered_set<RegisteredSystem*> on_stack;
    std::stack<RegisteredSystem*> stack;

    // TODO: This desperately needs to be moved to a separate class.
    std::function<void(RegisteredSystem*)> dfs;
    dfs = [&](RegisteredSystem* node) {
        if (visited.contains(node))
            return;

        visited.insert(node);
        on_stack.insert(node);

        for (auto neighbor : edges[node]) {
            dfs(const_cast<RegisteredSystem*>(neighbor));
        }

        on_stack.erase(node);
        stack.push(node);
    };

    for (const auto& system : vertices) {
        if (!visited.contains(system.get())) {
            dfs(system.get());
        }
    }

    // Lookup table for system pointers.
    std::unordered_map<RegisteredSystem*, std::unique_ptr<RegisteredSystem>> ptr_map;
    for (auto& system : vertices) {
        ptr_map[system.get()] = std::move(system);
    }

    m_systems.clear();
    while (!stack.empty()) {
        RegisteredSystem* ptr = stack.top();
        stack.pop();

        m_systems.emplace_back(std::move(ptr_map[ptr]));
    }


















    // Transfer ownership to the list.

    // Add iterator to the uninitialized systems queue.
    // const auto it = ;
    // m_uninitializedSystems.push(it);
}

template <InheritsISystem<> T>
void SystemScheduler::UnregisterSystem()
{
    m_systems.emplace_back(std::make_unique<T>());
}
