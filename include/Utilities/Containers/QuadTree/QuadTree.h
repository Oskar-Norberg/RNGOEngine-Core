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

        void AddNode(T data, Point position)
        {
            if (!CanContain(position))
            {
                return;
            }

            if (m_dataIndex >= CAPACITY && !m_subTrees.has_value())
            {
                Subdivide();
            }

            if (m_subTrees.has_value())
            {
                const auto childIndex = GetChildIndex(position);
                m_subTrees.value()[childIndex]->AddNode(data, position);
            }
            else
            {
                m_data[m_dataIndex].data = data;
                m_data[m_dataIndex].position = position;
                m_dataIndex++;
            }
        }

        std::vector<T> WithinRange(BoundingBox box) const
        {
            std::vector<T> result;

            WithinRangeRecursive(box, result);

            return result;
        }

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
        std::optional<std::array<std::unique_ptr<QuadTree<T, CAPACITY>>, 4>> m_subTrees;

        bool CanContain(const Point& point) const
        {
            return m_boundingBox.Contains(point);
        }

        void Subdivide()
        {
            GenerateSubTrees();

            // Emplace existing data into subtrees
            for (size_t i = 0; i < m_dataIndex; i++)
            {
                for (const auto& quadTree : m_subTrees.value())
                {
                    if (quadTree->CanContain(m_data[i].position))
                    {
                        quadTree->AddNode(m_data[i].data, m_data[i].position);
                        break;
                    }
                }
            }

            m_dataIndex = 0;
        }

        void GenerateSubTrees()
        {
            // Midpoint
            float midX = (m_boundingBox.start.x + m_boundingBox.end.x) / 2;
            float midY = (m_boundingBox.start.y + m_boundingBox.end.y) / 2;

            BoundingBox nwBox = {m_boundingBox.start, {midX, midY}};
            BoundingBox neBox = {{midX, m_boundingBox.start.y}, {m_boundingBox.end.x, midY}};
            BoundingBox swBox = {{m_boundingBox.start.x, midY}, {midX, m_boundingBox.end.y}};
            BoundingBox seBox = {{midX, midY}, m_boundingBox.end};

            m_subTrees = std::array<std::unique_ptr<QuadTree<T, CAPACITY>>, 4>{
                std::make_unique<QuadTree<T, CAPACITY>>(nwBox),
                std::make_unique<QuadTree<T, CAPACITY>>(neBox),
                std::make_unique<QuadTree<T, CAPACITY>>(swBox),
                std::make_unique<QuadTree<T, CAPACITY>>(seBox)
            };
        }

        QuadTreeDirection GetChildIndex(const Point& point) const
        {
            float midX = (m_boundingBox.start.x + m_boundingBox.end.x) / 2;
            float midY = (m_boundingBox.start.y + m_boundingBox.end.y) / 2;

            if (point.x <= midX)
                return (point.y <= midY) ? NORTH_WEST : SOUTH_WEST;

            return (point.y <= midY) ? NORTH_EAST : SOUTH_EAST;
        }

        void WithinRangeRecursive(BoundingBox boundingBox, std::vector<T>& result) const
        {
            if (!m_boundingBox.Intersects(boundingBox))
            {
                return;
            }

            for (size_t i = 0; i < m_dataIndex; i++)
            {
                if (boundingBox.Contains(m_data[i].position))
                {
                    result.push_back(m_data[i].data);
                }
            }

            // Query Subtrees
            if (m_subTrees.has_value())
            {
                for (const auto& subTree : m_subTrees.value())
                {
                    subTree->WithinRangeRecursive(boundingBox, result);
                }
            }
        }
    };
}