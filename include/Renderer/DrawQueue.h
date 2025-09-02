//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include <array>
#include <vector>

#include "RenderID.h"
#include "Components/Components.h"

namespace RNGOEngine::Core::Renderer
{
    struct Drawable
    {
        Components::Transform transform;
        MeshID mesh;
        MaterialID material;
    };

    // This is hard to reach from the GLSL shader part
    constexpr auto NR_OF_POINTLIGHTS = 16;
    constexpr auto NR_OF_SPOTLIGHTS = 16;


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
        // TODO: Combine these into CameraData.
        Components::Camera camera;
        Components::Transform cameraTransform;

        // Light Properties
        Components::AmbientLight ambientLight;
        Components::DirectionalLight directionalLight;
        
        std::array<PointLightData, NR_OF_POINTLIGHTS> pointLights;
        size_t pointLightIndex = 0;

        std::array<SpotlightData, NR_OF_POINTLIGHTS> spotlights;
        size_t spotlightIndex = 0;
        
        Components::BackgroundColor backgroundColor;

        // Opaque Objects
        std::vector<Drawable> opaqueObjects;

        // TODO: Transparents
    };
}