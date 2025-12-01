//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include <array>
#include <vector>

#include "Components/Components.h"
#include "Data/Shaders/ShaderSpecification.h"
#include "Uniforms.h"

namespace RNGOEngine::Core::Renderer
{
    struct Drawable
    {
        Components::Transform Transform;
        AssetHandling::AssetHandle ModelHandle;
        AssetHandling::AssetHandle Material;
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