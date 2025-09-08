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
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<Index> indices;
    };
}