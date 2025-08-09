//
// Created by Oskar.Norberg on 2025-08-08.
//

#pragma once

#include <array>
#include <memory>
#include <optional>
#include <vector>

namespace RNGOEngine
{

    enum QuadTreeDirection
    {
        NORTH_WEST = 0,
        NORTH_EAST = 1,
        SOUTH_WEST = 2,
        SOUTH_EAST = 3
    };

    struct Point
    {
        float x, y;
    };

    struct BoundingBox
    {
        Point start, end;

        bool Contains(Point point) const
        {
            return (point.x >= start.x && point.x <= end.x) &&
                   (point.y >= start.y && point.y <= end.y);
        }

        bool Intersects(const BoundingBox& other) const
        {
            return !(
                other.end.x < start.x || other.start.x > end.x || other.end.y < start.y ||
                other.start.y > end.y
            );
        }
    };

    template<typename T>
    struct QuadTreeNode
    {
        Point position;
        T data;
    };

    template<typename T, size_t CAPACITY>
    class QuadTree
    {
    public:
        explicit QuadTree(BoundingBox boundingBox)
            : m_boundingBox(boundingBox)
        {
        }

        void AddNode(T data, Point position);

        std::vector<T> WithinRange(BoundingBox box) const;

        // Only compile these in debug mode.
#ifndef NDEBUG
        BoundingBox GetBoundingBox() const
        {
            return m_boundingBox;
        }

        const std::optional<std::array<std::unique_ptr<QuadTree<T, CAPACITY>>, 4>>& GetSubTrees(
        ) const
        {
            return m_subTrees;
        }
#endif

    private:
        BoundingBox m_boundingBox;
        std::array<QuadTreeNode<T>, CAPACITY> m_data;
        size_t m_dataIndex = 0;
        std::array<std::unique_ptr<QuadTree<T, CAPACITY>>, 4> m_subTrees;
        size_t totalCapacity = 0;

        bool CanContain(const Point& point) const;

        void Subdivide();

        void GenerateSubTrees();

        QuadTreeDirection GetChildIndex(const Point& point) const;

        void WithinRangeRecursive(BoundingBox boundingBox, std::vector<T>& result) const;

        bool IsSubdivided() const;
    };

#include "Utilities/Containers/QuadTree/QuadTree.tpp"
}