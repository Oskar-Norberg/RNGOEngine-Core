#pragma once
#include <iostream>
#include <stack>

template <typename T>
void DirectedGraph<T>::AddVertex(T* vertex)
{
    // Vertex already exists, no need to add it again.
    if (vertices.contains(vertex))
    {
        return;
    }

    // Add vertex and initialize its edge set.
    vertices.insert(vertex);
    edges[vertex] = std::pmr::unordered_set<T>();
}

template <typename T>
void DirectedGraph<T>::AddEdge(T* from, T* to)
{
    edges[from].insert(to);
}

template <typename T>
std::vector<T*> DirectedGraph<T>::TopologicalSort()
{
    std::pmr::unordered_set<T*> visited;
    std::stack<T*> stack;

    // TODO: Detect cycles.

    // Visit all nodes using DFS.
    for (auto vertex : vertices)
    {
        if (!visited.contains(vertex))
        {
            dfs(vertex, visited, [&](T* node) { stack.push(node); });
        }
    }

    // Reverse the stack.
    std::vector<T*> sortedOrder;
    while (!stack.empty())
    {
        sortedOrder.push_back(stack.top());
        stack.pop();
    }

    return sortedOrder;
}

template <typename T>
void DirectedGraph<T>::dfs(T* node, std::pmr::unordered_set<T*>& visited, std::function<void(T*)> callback)
{
    if (visited.contains(node))
        return;

    visited.insert(node);

    for (auto neighbor : edges[node])
    {
        dfs(neighbor, visited, callback);
    }

    callback(node);
}
