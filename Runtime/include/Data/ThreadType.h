//
// Created by Oskar.Norberg on 2025-12-01.
//

#pragma once

namespace RNGOEngine::Data
{
    // TODO: I don't like how this is not an enum class. But I want to be able to use bitwise operations.
    enum ThreadType
    {
        Main = 1 << 0,
        Render = 1 << 1,
        // Worker = 1 << 2,
        // Audio 1 << 3,
    };
}