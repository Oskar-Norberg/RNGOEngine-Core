//
// Created by Oskar.Norberg on 2025-11-06.
//

#pragma once

namespace RNGOEngine::Core::Renderer
{
    enum RenderFeature : unsigned int
    {
        DepthTesting = 1 << 0,
        Blending = 1 << 1,
        BackFaceCulling = 1 << 2,
    };
}
