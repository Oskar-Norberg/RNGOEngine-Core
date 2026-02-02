//
// Created by Oskar.Norberg on 2025-08-08.
//

#pragma once

#include <array>
#include <memory>
#include <stack>
#include <vector>

#include "Math/2D/AABB2D.h"
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
    template<typename TData, typename TCoordinate>
    struct DataEntry
    {
        TData data;
        Math::AABB2D<TCoordinate> bounds;
    };

    template<typename TCoordinate>
    struct QuadTreeNode
    {
        // TODO: Data fragmentation because of vector. Unlucky.
        std::vector<NodeDataID> data;
        std::vector<NodeDataID> overflow;

        // See QuadTreeDirection for the ordering.
        std::array<NodeID, 4> children = {INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID};
        Math::AABB2D<TCoordinate> bounds;
    };

    ///
    /// @tparam TData Payload/data type stored in tree.
    /// @tparam TCoordinate Coordinate/Precision type for bounding box calculations.
    /// @tparam CAPACITY Max number of nodes in a quad.
    /// @brief Spatially partitioned QuadTree.
    ///
    template<typename TData, typename TCoordinate, size_t CAPACITY>
    class QuadTree
    {
    public:
        explicit QuadTree(size_t nrNodesEstimate, const Math::AABB2D<TCoordinate>& boundingBox)
        {
            if (nrNodesEstimate > 0)
            {
                // TODO: Find better heuristic
                m_trees.reserve(nrNodesEstimate / CAPACITY);
                m_data.reserve(nrNodesEstimate);
            }

            CreateNode(boundingBox);
        }

        explicit QuadTree(const Math::AABB2D<TCoordinate>& boundingBox)
            : QuadTree(0, boundingBox)
        {
        }

        ~QuadTree()
        {
            RNGO_ZONE_SCOPED_N("QuadTree Destructor");
        }

    public:
        void AddNode(TData data, const Math::AABB2D<TCoordinate>& bounds);
        void AddNode(TData&& data, const Math::AABB2D<TCoordinate>& bounds);

    public:
        inline std::vector<std::pair<TData, TData>> GetCollisionPairs() const;
        std::vector<std::pair<TData, TData>> GetCollisionPairs(NodeID id) const;

    public:
        inline const std::array<NodeID, 4>& GetChildren(NodeID id) const;
        inline bool IsSubdivided(NodeID id) const;

    private:
        std::vector<QuadTreeNode<TCoordinate>> m_trees{};

        std::vector<DataEntry<TData, TCoordinate>> m_data{};

        size_t totalCapacity = 0;

    private:
        inline void AddNode(DataID dataID, const Math::AABB2D<TCoordinate>& bounds);

    private:
        inline const QuadTreeNode<TCoordinate>& GetNode(NodeID id) const;

    private:
        inline void Subdivide(NodeID id);
        inline bool IsFull(NodeID id) const;

    private:
        inline void ClearNodeDataHandles(NodeID id);
        inline const DataEntry<TData, TCoordinate>& GetData(DataID id) const;
        inline const std::vector<DataID>& GetNodeDataHandles(NodeID id) const;
        inline const std::vector<DataID>& GetNodeOverflowHandles(NodeID id) const;

    private:
        DataID EmplaceData(TData data, const Math::AABB2D<TCoordinate>& bounds);
        DataID EmplaceData(TData&& data, const Math::AABB2D<TCoordinate>& bounds);

    private:
        // TODO: ID should really be the first parameter
        inline void AddDataToNode(TData&& data, const Math::AABB2D<TCoordinate>& bounds, NodeID id);
        inline void MoveDataToNode(DataID dataID, NodeID nodeID);

        inline void MoveDataToOverflow(DataID dataID, NodeID nodeID);

    private:
        inline NodeID CreateNode(const Math::AABB2D<TCoordinate>& bounds);

    private:
        inline void GenerateSubTrees(NodeID id);
        void TransferDataToChildren(NodeID id);

        inline void SetChildren(NodeID id, std::array<NodeID, 4> children);
    };

#include "Utilities/Containers/QuadTree/QuadTree.inl"
}