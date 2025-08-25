//
// Created by Oskar.Norberg on 2025-08-07.
//

#pragma once

namespace RNGOEngine::Events
{
    struct ExitEvent
    {
    };

    enum class KeyAction
    {
        Press   = 1 << 0,
        Release = 1 << 1
    };

    struct KeyEvent
    {
        int key;
        KeyAction action;
    };
}