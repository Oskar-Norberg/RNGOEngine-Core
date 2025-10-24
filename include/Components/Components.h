//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "glm/gtx/quaternion.hpp"

#include "AssetHandling/AssetManager/Managers/ModelManager.h"
#include "AssetHandling/AssetManager/Managers/Material/Material.h"

namespace RNGOEngine::Components
{
    struct MeshRenderer
    {
        Containers::Vectors::GenerationalKey<AssetHandling::ModelData> modelID;
        Containers::Vectors::GenerationalKey<AssetHandling::MaterialSpecification> materialID;
    };

    struct Transform
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = {1.0f, 1.0f, 1.0f};

        // TODO: Profile this versus just storing the matrix directly.
        glm::mat4 GetMatrix() const
        {
            auto m = glm::mat4(1.0f);
            m = glm::translate(m, position);
            m *= glm::toMat4(rotation);
            m = glm::scale(m, scale);

            return m;
        }
    };

    struct Camera
    {
        float fov = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
    };

    struct Color
    {
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    };

    struct Intensity
    {
        float intensity = 1.0f;
    };

    struct BackgroundColor
    {
    };

    struct LightFalloff
    {
        // Standard range is 100
        // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
        float constant = 1.0f;
        float linear = 0.045f;
        float quadratic = 0.0075f;
    };

    struct AmbientLight
    {
    };

    struct DirectionalLight
    {
    };

    struct PointLight
    {
    };

    struct Spotlight
    {
        float cutOff = glm::cos(glm::radians(10.0f));
        float outerCutOff = glm::cos(glm::radians(12.5f));
    };
}