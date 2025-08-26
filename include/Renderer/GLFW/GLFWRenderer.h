//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include "Renderer/IRenderer.h"

namespace RNGOEngine::Core::Renderer
{
    class GLFWRenderer : public IRenderer
    {
    public:
        GLFWRenderer();
        void Render(Window::IWindow& window) override;
    };
}