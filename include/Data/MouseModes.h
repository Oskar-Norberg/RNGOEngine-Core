//
// Created by ringo on 2025-10-04.
//

#pragma once

namespace RNGOEngine::Data::Mouse
{
    enum class MouseMode
    {
        Normal = 0,
        Hidden = 1 << 0,
        Locked = 1 << 1
    };
}