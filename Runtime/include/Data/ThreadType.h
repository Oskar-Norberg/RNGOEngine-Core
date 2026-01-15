//
// Created by Oskar.Norberg on 2025-12-01.
//

#pragma once
#include "Utilities/EnumDefinitions.h"

namespace RNGOEngine::Data
{
    enum class ThreadType
    {
        None = 0,
        Main = 1 << 0,
        Render = 1 << 1,
        // Worker = 1 << 2,
        // Audio 1 << 3,
    };
    DEFINE_ENUM_CLASS_BITWISE_OPERATORS(ThreadType)
}