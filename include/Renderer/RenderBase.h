//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "IRenderer.h"
#include "Uniforms.h"

namespace RNGOEngine::Core::Renderer
{
    class RenderBase : public IRenderer
    {
    public:
        void SubmitDrawQueue(const DrawQueue drawQueue) override
        {
            m_drawQueue = drawQueue;
        }

        DrawQueue m_drawQueue;
    };
}