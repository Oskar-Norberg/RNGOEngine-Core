//
// Created by Oskar.Norberg on 2025-08-08.
//

#pragma once

#include <array>
#include <memory>
#include <stack>
#include <vector>

#include "Math/BoundingBox.h"
#include "Math/Point.h"
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

    template<typename T>
    struct QuadTreeNode
    {
        T data;
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
        explicit QuadTree(const Math::BoundingBox& boundingBox)
            : m_boundingBox(boundingBox)
        {
        }

        ~QuadTree()
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree Destructor");
        }

        void AddNode(T data, const Math::BoundingBox& bounds);

        std::vector<std::pair<T, T>> GetCollisionPairs() const;

        // Only compile these in debug mode.
#ifndef NDEBUG
        Math::BoundingBox GetBoundingBox() const
        {
            return m_boundingBox;
        }

        const std::array<std::unique_ptr<QuadTree<T, CAPACITY>>, 4>& GetSubTrees(
        ) const
        {
            return m_subTrees;
        }
#endif

    private:
        Math::BoundingBox m_boundingBox;
        std::vector<QuadTreeNode<T>> m_data;
        std::array<std::unique_ptr<QuadTree<T, CAPACITY>>, 4> m_subTrees;
        size_t totalCapacity = 0;

        void Subdivide();

        void GenerateSubTrees();

        bool IsSubdivided() const;
    };

#include "Utilities/Containers/QuadTree/QuadTree.tpp"
}