//
// Created by Oskar.Norberg on 2025-10-20.
//

#pragma once

#include <vector>

#include "AssetManager/AssetManagers/TextureManager.h"
#include "Renderer/RenderID.h"
#include "Renderer/Uniforms.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::AssetHandling
{
    using MaterialID = unsigned int;
    constexpr auto INVALID_MATERIAL_ID = std::numeric_limits<MaterialID>::max();
    
    enum class MaterialParameterType { Bool, Int, Float, Vec2, Vec3, Vec4, Mat4, Texture };

    struct MaterialTextureSpecification
    {
        Containers::Vectors::GenerationalKey<TextureManagerData> textureKey;
        int slot;
    };

    union MaterialParameterData
    {
        bool b;
        int i;
        float f;
        glm::vec2 v2;
        glm::vec3 v3;
        glm::vec4 v4;
        glm::mat4 m4;
        MaterialTextureSpecification texture;
    };

    struct MaterialParameter
    {
        std::string name;
        MaterialParameterType type;
        MaterialParameterData data;
    };

    struct MaterialSpecification
    {
        Core::Renderer::ShaderProgramID shader;

        // Consider stack allocated array with max size?
        std::vector<MaterialParameter> uniforms;
    };
}