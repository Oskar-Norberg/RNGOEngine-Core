//
// Created by Oskar.Norberg on 2025-08-07.
//

#pragma once

#include "Data/MouseModes.h"

namespace RNGOEngine::Events
{
    struct ExitEvent
    {
    };

    enum class ButtonAction
    {
        Press   = 1 << 0,
        Release = 1 << 1
    };

    struct KeyEvent
    {
        int key;
        ButtonAction action;
    };

    struct MouseMoveEvent
    {
        double deltaX;
        double deltaY;
    };

    struct MouseButtonEvent
    {
        int button;
        ButtonAction action;
    };

    struct MouseModeEvent
    {
        Data::Mouse::MouseMode mode;
    };

    struct WindowSizeEvent
    {
        int width, height;
    };
}