//
// Created by SkummisDenAndre on 2025-06-20.
//

#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <stack>

namespace RNGOEngine::Containers::Graphs
{
    // TODO: Consider adding methods for removing vertices and edges. Not necessary for now.
    template<typename T>
    class DirectedGraph
    {
        using VertexPtr = T*;

    public:
        void AddVertex(VertexPtr vertex);

        void AddEdge(VertexPtr from, VertexPtr to);

        // Consider moving this to a DAG utilities class.
        std::vector<T*> TopologicalSort();

    private:
        std::pmr::unordered_set<VertexPtr> vertices;
        std::pmr::unordered_map<VertexPtr, std::pmr::unordered_set<VertexPtr>> edges;

        /// 
        /// @param node Starting node.
        /// @param visited Set of visited nodes.
        /// @param callback Callback function to call when a node is visited.
        ///
        void dfs(VertexPtr node, std::pmr::unordered_set<VertexPtr>& visited,
                 std::function<void(VertexPtr)> callback);
    };

#include "DirectedGraph.inl"
}