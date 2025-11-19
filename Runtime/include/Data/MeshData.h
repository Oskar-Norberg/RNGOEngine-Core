//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace RNGOEngine::Data::Rendering
{
    using Index = unsigned int;

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
    };

    struct MeshData
    {
        std::vector<Vertex> Vertices;
        std::vector<Index> Indices;
    };
}