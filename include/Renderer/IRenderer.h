//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include "Window/IWindow.h"

namespace RNGOEngine::Core::Renderer
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

    public:
        virtual void Render(Window::IWindow& window) = 0;
    };
}