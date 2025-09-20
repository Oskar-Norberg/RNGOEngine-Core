//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "RenderID.h"

namespace RNGOEngine::Core::Renderer
{
    enum class UniformType { Bool, Int, Float, Vec2, Vec3, Vec4, Mat4, Texture };

    struct TextureSpecification
    {
        TextureID texture;
        int slot;
    };

    union UniformData
    {
        bool b;
        int i;
        float f;
        glm::vec2 v2;
        glm::vec3 v3;
        glm::vec4 v4;
        glm::mat4 m4;
        TextureSpecification texture;
    };

    struct UniformSpecification
    {
        std::string name;
        UniformType type;
        UniformData data;
    };

    struct MaterialSpecification
    {
        ShaderProgramID shader;
        
        // Consider stack allocated array with max size?
        std::vector<UniformSpecification> uniforms;
    };
}