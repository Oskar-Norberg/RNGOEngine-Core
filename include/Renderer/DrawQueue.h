//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include <array>
#include <vector>

#include "RenderID.h"
#include "Uniforms.h"
#include "Components/Components.h"

namespace RNGOEngine::Core::Renderer
{
    constexpr auto NR_OF_POINTLIGHTS = 16;
    constexpr auto NR_OF_SPOTLIGHTS = 16;
    
    struct Drawable
    {
        Components::Transform transform;
        Containers::Vectors::GenerationalKey<AssetHandling::ModelData> modelID;
        MaterialID material;
    };

    struct CameraData
    {
        Components::Transform transform;
        
        float fov;
        float nearPlane;
        float farPlane;
    };

    struct BackgroundColorData
    {
        glm::vec3 color;
    };

    struct AmbientLightData
    {
        glm::vec3 color;
        float intensity;
    };

    struct DirectionalLightData
    {
        glm::vec3 color;
        float intensity;
        glm::vec3 direction;
    };

    struct PointLightData
    {
        glm::vec3 color;
        float intensity;

        glm::vec3 position;
        float constant;

        float linear;
        float quadratic;
    };

    struct SpotlightData
    {
        glm::vec3 color;
        float intensity;

        glm::vec3 position;
        float cutoff;
        glm::vec3 direction;

        float outerCutoff;

        float constant;
        float linear;
        float quadratic;
    };

    struct DrawQueue
    {
        // Camera Properties
        CameraData camera;

        // Light Properties
        AmbientLightData ambientLight;
        DirectionalLightData directionalLight;

        std::array<PointLightData, NR_OF_POINTLIGHTS> pointLights;
        size_t pointLightIndex = 0;

        std::array<SpotlightData, NR_OF_POINTLIGHTS> spotlights;
        size_t spotlightIndex = 0;

        BackgroundColorData backgroundColor;

        // Opaque Objects
        std::vector<Drawable> opaqueObjects;

        // TODO: Transparents
    };
}