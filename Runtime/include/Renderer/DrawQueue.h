//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include <array>
#include <variant>
#include <vector>

// TODO: This should probably not be linked directly to Components. Make a GPUTransform?
#include "Components/Components.h"
#include "Data/Shaders/ShaderSpecification.h"
#include "RenderID.h"

namespace RNGOEngine::Core::Renderer
{
    struct GPUMaterialTextureSpecification
    {
        TextureID TextureHandle;
        int Slot;
    };

    struct GPUMaterialParameter
    {
        std::string Name;
        std::variant<
            bool, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, GPUMaterialTextureSpecification>
            Data;
    };

    struct GPUMaterial
    {
        ShaderProgramID ShaderProgram;
        std::vector<GPUMaterialParameter> Parameters;
    };

    struct GPUMesh
    {
        VAO VAO = INVALID_VAO;
        VBO VBO = INVALID_VBO;
        EBO EBO = INVALID_EBO;
        size_t ElementCount = 0;
    };

    struct GPUModel
    {
        std::vector<GPUMesh> Meshes;
    };

    struct Drawable
    {
        Components::Transform Transform;
        GPUModel Model;
        GPUMaterial Material;
    };

    struct CameraData
    {
        Components::Transform Transform;

        float FOV;
        float NearPlane;
        float FarPlane;
    };

    struct BackgroundColorData
    {
        glm::vec3 Color;
    };

    struct AmbientLightData
    {
        glm::vec3 Color;
        float Intensity;
    };

    struct DirectionalLightData
    {
        glm::vec3 Color;
        float Intensity;
        glm::vec3 Direction;
    };

    struct PointLightData
    {
        glm::vec3 Color;
        float Intensity;

        glm::vec3 Position;
        float Constant;

        float Linear;
        float Quadratic;
    };

    struct SpotlightData
    {
        glm::vec3 Color;
        float Intensity;

        glm::vec3 Position;
        float Cutoff;
        glm::vec3 Direction;

        float OuterCutoff;

        float Constant;
        float Linear;
        float Quadratic;
    };

    struct DrawQueue
    {
        // Camera Properties
        CameraData Camera;

        // Light Properties
        AmbientLightData AmbientLight;
        DirectionalLightData DirectionalLight;

        std::array<PointLightData, Data::Shader::NR_OF_POINTLIGHTS.Value> PointLights;
        size_t PointLightIndex = 0;

        std::array<SpotlightData, Data::Shader::NR_OF_SPOTLIGHTS.Value> Spotlights;
        size_t SpotlightIndex = 0;

        BackgroundColorData BackgroundColor;

        // Opaque Objects
        std::vector<Drawable> OpaqueObjects;

        // TODO: Transparents
    };
}