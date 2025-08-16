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

TEST(QuadTreeTests, NoDuplicateCollisionPairs)
{
    constexpr int size = 50;
    constexpr float sizeFloat = static_cast<float>(size);

    RNGOEngine::Containers::Graphs::QuadTree<int, 16> quadTree(
        RNGOEngine::Math::BoundingBox{{0.0f, 0.0f}, {sizeFloat, sizeFloat}});

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int index = i * size + j;
            quadTree.AddNode(index, {static_cast<float>(i), static_cast<float>(j)});
        }
    }

    const auto collisionPairs = quadTree.GetCollisionPairs();

    std::unordered_set<std::pair<int, int>, PairHash> uniquePairs;

    for (auto collisionPair : collisionPairs)
    {
        std::pair<float, float> normalizedPair = {
            std::min(collisionPair.first, collisionPair.second),
            std::max(collisionPair.first, collisionPair.second)
        };

        ASSERT_EQ(uniquePairs.contains(normalizedPair), false);

        uniquePairs.insert(normalizedPair);
    }
}
