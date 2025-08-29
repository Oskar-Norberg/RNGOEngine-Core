//
// Created by Oskar.Norberg on 2025-08-29.
//

#pragma once

// Taken from GLFW and converted to constexpr.
namespace RNGOEngine::Data::MouseCodes
{
    constexpr auto RNGO_MOUSE_BUTTON_1          = 0;
    constexpr auto RNGO_MOUSE_BUTTON_2          = 1;
    constexpr auto RNGO_MOUSE_BUTTON_3          = 2;
    constexpr auto RNGO_MOUSE_BUTTON_4          = 3;
    constexpr auto RNGO_MOUSE_BUTTON_5          = 4;
    constexpr auto RNGO_MOUSE_BUTTON_6          = 5;
    constexpr auto RNGO_MOUSE_BUTTON_7          = 6;
    constexpr auto RNGO_MOUSE_BUTTON_8          = 7;
    constexpr auto RNGO_MOUSE_BUTTON_LAST   = RNGO_MOUSE_BUTTON_8;
    constexpr auto RNGO_MOUSE_BUTTON_LEFT   = RNGO_MOUSE_BUTTON_1;
    constexpr auto RNGO_MOUSE_BUTTON_RIGHT  = RNGO_MOUSE_BUTTON_2;
    constexpr auto RNGO_MOUSE_BUTTON_MIDDLE = RNGO_MOUSE_BUTTON_3;
}