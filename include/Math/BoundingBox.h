//
// Created by Oskar.Norberg on 2025-08-13.
//

#pragma once

#include "Point.h"

namespace RNGOEngine::Math
{
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
}