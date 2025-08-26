//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "Renderer/IRenderer.h"

namespace RNGOEngine::Components
{
    struct MeshRenderer
    {
        Core::Renderer::MeshHandle mesh;
        Core::Renderer::ShaderHandle shader;
    };
}