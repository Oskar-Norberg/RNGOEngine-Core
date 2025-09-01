//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

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

    struct PointLightData
    {
        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;

        glm::vec3 position;
        float constant = 1.0f;
        
        float linear = 0.045f;
        float quadratic = 0.0075f;
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
        // TODO: SpotLight.
        // TODO: Skybox/SceneColor

        // Opaque Objects
        std::vector<Drawable> opaqueObjects;

        // TODO: Transparents
    };
}