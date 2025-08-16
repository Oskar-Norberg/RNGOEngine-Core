//
// Created by ringo on 2025-08-16.
//

#include <gtest/gtest.h>

#include <unordered_set>

#include "Utilities/Containers/QuadTree/QuadTree.h"

struct PairHash
{
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

TEST(QuadTreeTests, CollisionsNotZero)
{
    constexpr int size = 100;
    constexpr float sizeFloat = static_cast<float>(size);

    RNGOEngine::Containers::Graphs::QuadTree<int, 16> quadTree(
        RNGOEngine::Math::BoundingBox{{0.0f, 0.0f}, {sizeFloat, sizeFloat}});

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int index = i * size + j;

            RNGOEngine::Math::BoundingBox bounds = {
                {static_cast<float>(i), static_cast<float>(j)},
                {i + 10.0f, j + 10.0f}
            };

            quadTree.AddNode(index, bounds);
        }
    }

    const auto collisionPairs = quadTree.GetCollisionPairs();

    ASSERT_GT(collisionPairs.size(), 0);
}

TEST(QuadTreeTests, NoDuplicateCollisionPairs)
{
    constexpr int size = 25;
    constexpr float sizeFloat = static_cast<float>(size);

    RNGOEngine::Containers::Graphs::QuadTree<int, 16> quadTree(
        RNGOEngine::Math::BoundingBox{{0.0f, 0.0f}, {sizeFloat, sizeFloat}});

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int index = i * size + j;

            RNGOEngine::Math::BoundingBox bounds = {
                {static_cast<float>(i), static_cast<float>(j)},
                {i + 10.0f, j + 10.0f}
            };

            quadTree.AddNode(index, bounds);
        }
    }

    const auto collisionPairs = quadTree.GetCollisionPairs();

    std::unordered_set<std::pair<int, int>, PairHash> uniquePairs;

    for (auto collisionPair : collisionPairs)
    {
        std::pair<int, int> normalizedPair = {
            std::min(collisionPair.first, collisionPair.second),
            std::max(collisionPair.first, collisionPair.second)
        };

        ASSERT_EQ(uniquePairs.contains(normalizedPair), false);

        uniquePairs.insert(normalizedPair);
    }
}

TEST(QuadTreeTests, NoSelfCollisions)
{
    constexpr int size = 100;
    constexpr float sizeFloat = static_cast<float>(size);

    RNGOEngine::Containers::Graphs::QuadTree<int, 16> quadTree(
        RNGOEngine::Math::BoundingBox{{0.0f, 0.0f}, {sizeFloat, sizeFloat}});

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int index = i * size + j;

            RNGOEngine::Math::BoundingBox bounds = {
                {static_cast<float>(i), static_cast<float>(j)},
                {i + 10.0f, j + 10.0f}
            };

            quadTree.AddNode(index, bounds);
        }
    }

    const auto collisionPairs = quadTree.GetCollisionPairs();

    std::unordered_set<std::pair<int, int>, PairHash> uniquePairs;

    for (auto collisionPair : collisionPairs)
    {
        ASSERT_EQ(collisionPair.first == collisionPair.second, false);
    }
}