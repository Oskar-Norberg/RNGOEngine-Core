//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "Renderer/IRenderer.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

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
        // TODO: Store as a quaternion?
        glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 scale = {1.0f, 1.0f, 1.0f};

        // TODO: Profile this versus just storing the matrix directly.
        glm::mat4 GetMatrix() const
        {
            glm::mat4 m = glm::mat4(1.0f);
            m = glm::translate(m, position);
            m = glm::rotate(m, rotation.x, {1.0f, 0.0f, 0.0f});
            m = glm::rotate(m, rotation.y, {0.0f, 1.0f, 0.0f});
            m = glm::rotate(m, rotation.z, {0.0f, 0.0f, 1.0f});
            m = glm::scale(m, scale);

            return m;
        }
    };
}