//
// Created by Oskar.Norberg on 2026-01-26.
//

#pragma once

#include <yaml-cpp/yaml.h>

#include "Components.h"

namespace RNGOEngine::Components
{
    static MeshRenderer DeserializeMeshRenderer(const YAML::Node& node)
    {
        MeshRenderer meshRenderer;

        meshRenderer.ModelHandle = AssetHandling::AssetHandle(node["ModelHandle"].as<uint64_t>());
        meshRenderer.MaterialKey = AssetHandling::AssetHandle(node["MaterialKey"].as<uint64_t>());

        return meshRenderer;
    }

    static Transform DeserializeTransform(const YAML::Node& node)
    {
        Transform transform;

        auto positionNode = node["Position"];
        transform.Position =
            glm::vec3(positionNode[0].as<float>(), positionNode[1].as<float>(), positionNode[2].as<float>());

        auto rotationNode = node["Rotation"];
        transform.Rotation = glm::quat(
            rotationNode[0].as<float>(), rotationNode[1].as<float>(), rotationNode[2].as<float>(),
            rotationNode[3].as<float>()
        );

        auto scaleNode = node["Scale"];
        transform.Scale =
            glm::vec3(scaleNode[0].as<float>(), scaleNode[1].as<float>(), scaleNode[2].as<float>());

        return transform;
    }

    static Camera DeserializeCamera(const YAML::Node& node)
    {
        Camera camera;

        camera.FOV = node["FOV"].as<float>();
        camera.NearPlane = node["NearPlane"].as<float>();
        camera.FarPlane = node["FarPlane"].as<float>();

        return camera;
    }

    static SphereCollider DeserializeSphereCollider(const YAML::Node& node)
    {
        SphereCollider sphereCollider;
        sphereCollider.Radius = node["Radius"].as<float>();

        return sphereCollider;
    }

    static Color DeserializeColor(const YAML::Node& node)
    {
        Color color;

        auto colorNode = node["ColorValue"];
        color.ColorValue =
            glm::vec3(colorNode[0].as<float>(), colorNode[1].as<float>(), colorNode[2].as<float>());

        return color;
    }

    static Intensity DeserializeIntensity(const YAML::Node& node)
    {
        Intensity intensity;

        intensity.IntensityValue = node["IntensityValue"].as<float>();

        return intensity;
    }

    static LightFalloff DeserializeLightFalloff(const YAML::Node& node)
    {
        LightFalloff falloff;

        falloff.Constant = node["Constant"].as<float>();
        falloff.Linear = node["Linear"].as<float>();
        falloff.Quadratic = node["Quadratic"].as<float>();

        return falloff;
    }

    static Spotlight DeserializeSpotlight(const YAML::Node& node)
    {
        Spotlight spotlight;

        spotlight.CutOff = node["CutOff"].as<float>();
        spotlight.OuterCutOff = node["OuterCutOff"].as<float>();

        return spotlight;
    }

    static Name DeserializeName(const YAML::Node& node)
    {
        Name name;

        std::string nameStr = node["Name"].as<std::string>();
        std::copy(nameStr.begin(), nameStr.end(), name.NameArr.begin());
        name.NameArr[std::min(nameStr.size(), name.NameArr.size() - 1)] = '\0';

        return name;
    }

    // Don't have functions for the tag components, do that straight in the Scene.
}
