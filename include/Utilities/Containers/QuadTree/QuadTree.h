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

    using NodeDataID = NodeID;
    constexpr NodeDataID INVALID_NODE_DATA_ID = std::numeric_limits<NodeDataID>::max();

    struct DataHandle
    {
        NodeDataID dataID;
        NodeDataID boundsID;
    };

    // TODO: T is not being used.
    template<typename T, size_t CAPACITY>
    struct QuadTreeNode
    {
        std::array<DataHandle, CAPACITY> data;
        size_t dataIndex = 0;
        
        std::array<NodeID, 4> children;
        NodeDataID boundsHandle;
    };

    // TODO: Add separate splitting size along with max capacity.

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
        std::vector<Math::BoundingBox> m_treeBounds;

        std::vector<T> m_data;
        std::vector<Math::BoundingBox> m_dataBounds;
        
        size_t totalCapacity = 0;
        
    private:
        const std::array<NodeID, 4>& GetChildren(NodeID id) const;
        void Subdivide(NodeID id);
        bool IsFull(NodeID id) const;
        bool IsSubdivided(NodeID id) const;

    private:
        const Math::BoundingBox& GetNodeBounds(NodeID id) const;

    private:
        void ClearNodeDataHandles(NodeID id);
        const T& GetData(DataHandle id) const;
        const Math::BoundingBox& GetDataBounds(DataHandle id) const;
        std::pair<const std::array<DataHandle, CAPACITY>&, size_t> GetNodeDataHandles(NodeID id) const;
        // TODO: ID should really be the first parameter
        void AddDataToNode(T data, const Math::BoundingBox& bounds, NodeID id);
        void MoveDataToNode(DataHandle dataHandle, NodeID nodeID);

    private:
        NodeID CreateNode(const Math::BoundingBox& bounds);
        void GenerateSubTrees(NodeID id);

        void SetChildren(NodeID id, std::array<NodeID, 4> children);
    };

#include "Utilities/Containers/QuadTree/QuadTree.tpp"
}