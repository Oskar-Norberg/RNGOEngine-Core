//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "glm/gtx/quaternion.hpp"

#include "Assets/Asset.h"

namespace RNGOEngine::Components
{
    struct MeshRenderer
    {
        AssetHandling::AssetHandle ModelHandle;
        AssetHandling::AssetHandle MaterialKey;
    };

    struct Transform
    {
        glm::vec3 Position = {0.0f, 0.0f, 0.0f};
        glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

        // TODO: Profile this versus just storing the matrix directly.
        glm::mat4 GetMatrix() const
        {
            auto m = glm::mat4(1.0f);
            m = glm::translate(m, Position);
            m *= glm::toMat4(Rotation);
            m = glm::scale(m, Scale);

            return m;
        }
    };

    struct Camera
    {
        float FOV = 45.0f;
        float NearPlane = 0.1f;
        float FarPlane = 100.0f;
    };

    struct Color
    {
        glm::vec3 ColorValue = glm::vec3(1.0f, 1.0f, 1.0f);
    };

    struct Intensity
    {
        float IntensityValue = 1.0f;
    };

    struct BackgroundColor
    {
    };

    struct LightFalloff
    {
        // Standard range is 100
        // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
        float Constant = 1.0f;
        float Linear = 0.045f;
        float Quadratic = 0.0075f;
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
        float CutOff = glm::cos(glm::radians(10.0f));
        float OuterCutOff = glm::cos(glm::radians(12.5f));
    };

    // TODO: Implement a custom FixedString templated-type?
    constexpr auto MAX_NR_CHARACTERS = 24 + 1;
    constexpr auto UNNAMED_ENTITY = "Unnamed Entity";
    struct Name
    {
        std::array<char, MAX_NR_CHARACTERS> NameArr;
    };
}