//
// Created by Oskar.Norberg on 2025-08-13.
//

#pragma once

#include "Utilities/Concepts/Concepts.h"

namespace RNGOEngine::Math
{
    template<typename T>
        requires Concepts::IsArithmetic<T>
    struct Point2D
    {
        T X, Y;
    };
}