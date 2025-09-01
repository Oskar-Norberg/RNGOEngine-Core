//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "Renderer/IRenderer.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "glm/gtx/quaternion.hpp"

namespace RNGOEngine::Components
{
    struct MeshRenderer
    {
        Core::Renderer::MeshID mesh;
        Core::Renderer::MaterialID shader;
    };

    struct Transform
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = {1.0f, 1.0f, 1.0f};

        // TODO: Profile this versus just storing the matrix directly.
        glm::mat4 GetMatrix() const
        {
            glm::mat4 m = glm::mat4(1.0f);
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

    struct AmbientLight
    {
        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
    };

    struct DirectionalLight
    {
        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
        glm::vec3 direction = {1.0f, 1.0f, 1.0f};
    };
}