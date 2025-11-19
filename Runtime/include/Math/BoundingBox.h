//
// Created by Oskar.Norberg on 2025-08-13.
//

#pragma once

#include "Point.h"

namespace RNGOEngine::Math
{
    struct BoundingBox
    {
        Point Start, End;

        bool Contains(Point point) const
        {
            return (point.x >= Start.x && point.x <= End.x) &&
                   (point.y >= Start.y && point.y <= End.y);
        }

        bool Contains(const BoundingBox& other) const
        {
            return Contains(other.Start) && Contains(other.End);
        }

        bool Intersects(const BoundingBox& other) const
        {
            return !(
                other.End.x < Start.x || other.Start.x > End.x || other.End.y < Start.y ||
                other.Start.y > End.y
            );
        }
    };
}