//
// Created by ringo on 2026-01-27.
//

#pragma once

#include "Utilities/Concepts/Concepts.h"

namespace RNGOEngine::Math
{
    template<typename T>
        requires Concepts::IsArithmetic<T>
    struct Point3D
    {
        T X, Y, Z;
    };
}