//
// Created by Oskar.Norberg on 2026-01-26.
//

#pragma once

#include "Components.h"
#include "yaml-cpp/emitter.h"

namespace RNGOEngine::Components
{
    static void SerializeMeshRenderer(const MeshRenderer& meshRenderer, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "MeshRenderer" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "ModelHandle" << YAML::Value << meshRenderer.ModelHandle.GetValue();
        emitter << YAML::Key << "MaterialKey" << YAML::Value << meshRenderer.MaterialKey.GetValue();
        emitter << YAML::EndMap;
    }

    static void SerializeTransform(const Transform& transform, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "Position" << YAML::Value << YAML::Flow << YAML::BeginSeq
                << transform.Position.x << transform.Position.y << transform.Position.z << YAML::EndSeq;

        emitter << YAML::Key << "Rotation" << YAML::Value << YAML::Flow << YAML::BeginSeq
                << transform.Rotation.w << transform.Rotation.x << transform.Rotation.y
                << transform.Rotation.z << YAML::EndSeq;

        emitter << YAML::Key << "Scale" << YAML::Value << YAML::Flow << YAML::BeginSeq << transform.Scale.x
                << transform.Scale.y << transform.Scale.z << YAML::EndSeq;
        emitter << YAML::EndMap;
    }

    static void SerializeCamera(const Camera& camera, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "FOV" << YAML::Value << camera.FOV;
        emitter << YAML::Key << "NearPlane" << YAML::Value << camera.NearPlane;
        emitter << YAML::Key << "FarPlane" << YAML::Value << camera.FarPlane;
        emitter << YAML::EndMap;
    }

    static void SerializeSphereCollider(const SphereCollider& sphereCollider, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "SphereCollider" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "Radius" << YAML::Value << sphereCollider.Radius;
        emitter << YAML::EndMap;
    }

    static void SerializeColor(const Color& color, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "Color" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "ColorValue" << YAML::Value << YAML::Flow << YAML::BeginSeq
                << color.ColorValue.x << color.ColorValue.y << color.ColorValue.z << YAML::EndSeq;
        emitter << YAML::EndMap;
    }

    static void SerializeIntensity(const Intensity& intensity, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "Intensity" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "IntensityValue" << YAML::Value << intensity.IntensityValue;
        emitter << YAML::EndMap;
    }

    static void SerializeLightFalloff(const LightFalloff& falloff, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "LightFalloff" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "Constant" << YAML::Value << falloff.Constant;
        emitter << YAML::Key << "Linear" << YAML::Value << falloff.Linear;
        emitter << YAML::Key << "Quadratic" << YAML::Value << falloff.Quadratic;
        emitter << YAML::EndMap;
    }

    static void SerializeSpotlight(const Spotlight& spotlight, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "Spotlight" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "CutOff" << YAML::Value << spotlight.CutOff;
        emitter << YAML::Key << "OuterCutOff" << YAML::Value << spotlight.OuterCutOff;
        emitter << YAML::EndMap;
    }

    static void SerializeName(const Name& name, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "Name" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "Name" << YAML::Value << name.NameArr.data();
        emitter << YAML::EndMap;
    }

    // Tag Components
    static void SerializeBackgroundColor(YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "BackgroundColor" << YAML::Value << true;
    }

    static void SerializeAmbientLight(YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "AmbientLight" << YAML::Value << true;
    }

    static void SerializeDirectionalLight(YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "DirectionalLight" << YAML::Value << true;
    }

    static void SerializePointLight(YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "PointLight" << YAML::Value << true;
    }
}
