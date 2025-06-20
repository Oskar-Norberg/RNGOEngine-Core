//
// Created by SkummisDenAndre on 2025-06-20.
//

#pragma once

#include <functional>
#include <stack>
#include <unordered_map>
#include <unordered_set>

// TODO: Consider adding methods for removing vertices and edges. Not necessary for now.
template <typename T>
class DirectedGraph
{
public:
    void AddVertex(T* vertex);

    void AddEdge(T* from, T* to);

    // Consider moving this to a DAG utilities class.
    std::vector<T*> TopologicalSort();

private:
    std::pmr::unordered_set<T> vertices;
    std::pmr::unordered_map<T, std::pmr::unordered_set<T>> edges;

    /// 
    /// @param node Starting node.
    /// @param visited Set of visited nodes.
    /// @param callback Callback function to call when a node is visited.
    ///
    void dfs(T* node, std::pmr::unordered_set<T*>& visited, std::function<void(T*)> callback);
};

#include "DirectedGraph.tpp"
