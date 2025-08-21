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


    // ID of the node itself.
    using NodeID = size_t;
    constexpr NodeID ROOT_NODE_ID = 0;
    constexpr NodeID INVALID_NODE_ID = std::numeric_limits<NodeID>::max();

    // ID for data pertaining to the node such as the bounds.
    using NodeDataID = NodeID;
    constexpr NodeDataID INVALID_NODE_DATA_ID = std::numeric_limits<NodeDataID>::max();

    // ID for the data stored in the node.
    using DataID = NodeDataID;

    // Store these in a struct because they will almost always be accessed together.
    template<typename T>
    struct DataEntry
    {
        T data;
        Math::BoundingBox bounds;
    };

    struct QuadTreeNode
    {
        // TODO: Data fragmentation because of vector. Unlucky.
        std::vector<NodeDataID> data;
        std::vector<NodeDataID> overflow;

        // See QuadTreeDirection for the ordering.
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
                m_data.reserve(nrNodesEstimate);
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

    public:
        void AddNode(T&& data, const Math::BoundingBox& bounds);

    public:
        inline std::vector<std::pair<T, T>> GetCollisionPairs() const;
        std::vector<std::pair<T, T>> GetCollisionPairs(NodeID id) const;

    public:
        inline const std::array<NodeID, 4>& GetChildren(NodeID id) const;
        inline bool IsSubdivided(NodeID id) const;

    private:
        std::vector<QuadTreeNode> m_trees;

        std::vector<DataEntry<T>> m_data;

        size_t totalCapacity = 0;

    private:
        inline const QuadTreeNode& GetNode(NodeID id) const;

    private:
        inline void Subdivide(NodeID id);
        inline bool IsFull(NodeID id) const;
    private:
        inline void ClearNodeDataHandles(NodeID id);
        inline const DataEntry<T>& GetData(DataID id) const;
        inline const std::vector<DataID>& GetNodeDataHandles(NodeID id) const;
        inline const std::vector<DataID>& GetNodeOverflowHandles(NodeID id) const;

    private:
        DataID EmplaceData(T&& data, const Math::BoundingBox& bounds);

    private:
        // TODO: ID should really be the first parameter
        inline void AddDataToNode(T&& data, const Math::BoundingBox& bounds, NodeID id);
        inline void MoveDataToNode(DataID dataID, NodeID nodeID);

        inline void MoveDataToOverflow(DataID dataID, NodeID nodeID);

    private:
        inline NodeID CreateNode(const Math::BoundingBox& bounds);

    private:
        inline void GenerateSubTrees(NodeID id);
        void TransferDataToChildren(NodeID id);

        inline void SetChildren(NodeID id, std::array<NodeID, 4> children);
    };

#include "Utilities/Containers/QuadTree/QuadTree.tpp"
}