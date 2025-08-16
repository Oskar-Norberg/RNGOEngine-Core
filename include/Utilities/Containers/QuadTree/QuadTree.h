//
// Created by Oskar.Norberg on 2025-08-08.
//

#pragma once

#include <array>
#include <memory>
#include <stack>
#include <vector>

#include "Math/BoundingBox.h"
#include "Profiling/Profiling.h"
#include "Utilities/Hash/PairHash.h"

namespace RNGOEngine::Containers::Graphs
{
    enum QuadTreeDirection
    {
        NORTH_WEST = 0,
        NORTH_EAST = 1,
        SOUTH_WEST = 2,
        SOUTH_EAST = 3
    };

    using NodeID = size_t;
    constexpr NodeID ROOT_NODE_ID = 0;
    constexpr NodeID INVALID_NODE_ID = std::numeric_limits<NodeID>::max();

    template<typename T>
    struct QuadTreeData
    {
        T data;
        Math::BoundingBox bounds;
    };

    template<typename T, size_t CAPACITY>
    struct QuadTreeNode
    {
        std::vector<QuadTreeData<T>> data;
        std::array<NodeID, 4> children;
        Math::BoundingBox bounds;
    };

    /// 
    /// @tparam T Payload/data type stored in tree.
    /// @tparam CAPACITY Max number of nodes in a quad.
    /// @brief Spatially partitioned QuadTree.
    ///
    template<typename T, size_t CAPACITY>
    class QuadTree
    {
    public:
        explicit QuadTree(size_t nrNodesEstimate, const Math::BoundingBox& boundingBox)
        {
            if (nrNodesEstimate > 0)
            {
                // TODO: Find better heuristic
                m_trees.reserve(nrNodesEstimate / CAPACITY);
            }
            
            CreateNode(boundingBox);
        }
        
        explicit QuadTree(const Math::BoundingBox& boundingBox)
            : QuadTree(0, boundingBox)
        {
        }

        ~QuadTree()
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree Destructor");
        }

        void AddNode(T data, const Math::BoundingBox& bounds);

        std::vector<std::pair<T, T>> GetCollisionPairs() const;

    private:
        std::vector<QuadTreeNode<T, CAPACITY>> m_trees;
        size_t totalCapacity = 0;

        std::vector<QuadTreeData<T>> GetNodeData(NodeID id) const;
        // TODO: ID should really be the first parameter
        void SetNodeData(std::vector<QuadTreeData<T>>&& data, NodeID id);
        void AddDataToNode(T data, const Math::BoundingBox& bounds, NodeID id);
        Math::BoundingBox GetBoundingBox(NodeID id) const;
        std::array<NodeID, 4> GetChildren(NodeID id) const;
        void Subdivide(NodeID id);
        bool IsFull(NodeID id) const;
        bool IsSubdivided(NodeID id) const;

    private:
        NodeID CreateNode(const Math::BoundingBox& bounds);
        NodeID CreateNode(T data, const Math::BoundingBox& bounds);
        void GenerateSubTrees(NodeID id);

        void SetChildren(NodeID id, std::array<NodeID, 4> children);
    };

#include "Utilities/Containers/QuadTree/QuadTree.tpp"
}