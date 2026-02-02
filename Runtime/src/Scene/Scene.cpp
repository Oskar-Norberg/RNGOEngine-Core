//
// Created by Oskar.Norberg on 2026-01-26.
//

#include "Scene/Scene.h"

#include "Assets/AssetLoader.h"
#include "ECS/Components/ComponentDeserializers.h"
#include "ECS/Components/ComponentSerializers.h"

namespace RNGOEngine::Core
{
    static void SerializeComponents(
        const entt::registry& registry, const entt::entity entity, YAML::Emitter& emitter
    )
    {
        // TODO: Garbage ass duplicate code
        if (registry.any_of<Components::MeshRenderer>(entity))
        {
            Components::SerializeMeshRenderer(registry.get<Components::MeshRenderer>(entity), emitter);
        }
        if (registry.any_of<Components::Transform>(entity))
        {
            Components::SerializeTransform(registry.get<Components::Transform>(entity), emitter);
        }
        if (registry.any_of<Components::Camera>(entity))
        {
            Components::SerializeCamera(registry.get<Components::Camera>(entity), emitter);
        }
        if (registry.any_of<Components::SphereCollider>(entity))
        {
            Components::SerializeSphereCollider(registry.get<Components::SphereCollider>(entity), emitter);
        }
        if (registry.any_of<Components::Rigidbody>(entity))
        {
            Components::SerializeRigidbody(registry.get<Components::Rigidbody>(entity), emitter);
        }
        if (registry.any_of<Components::Color>(entity))
        {
            Components::SerializeColor(registry.get<Components::Color>(entity), emitter);
        }
        if (registry.any_of<Components::Intensity>(entity))
        {
            Components::SerializeIntensity(registry.get<Components::Intensity>(entity), emitter);
        }
        if (registry.any_of<Components::LightFalloff>(entity))
        {
            Components::SerializeLightFalloff(registry.get<Components::LightFalloff>(entity), emitter);
        }
        if (registry.any_of<Components::Spotlight>(entity))
        {
            Components::SerializeSpotlight(registry.get<Components::Spotlight>(entity), emitter);
        }
        if (registry.any_of<Components::Name>(entity))
        {
            Components::SerializeName(registry.get<Components::Name>(entity), emitter);
        }

        // Tag Components
        if (registry.any_of<Components::BackgroundColor>(entity))
        {
            Components::SerializeBackgroundColor(emitter);
        }
        if (registry.any_of<Components::DirectionalLight>(entity))
        {
            Components::SerializeDirectionalLight(emitter);
        }
        if (registry.any_of<Components::PointLight>(entity))
        {
            Components::SerializePointLight(emitter);
        }
    }

    static void DeserializeComponents(entt::registry& registry, const entt::entity entity, YAML::Node& node)
    {
        if (node["MeshRenderer"])
        {
            auto meshRenderer = Components::DeserializeMeshRenderer(node["MeshRenderer"]);
            registry.emplace<Components::MeshRenderer>(entity, meshRenderer);
        }
        if (node["Transform"])
        {
            auto transform = Components::DeserializeTransform(node["Transform"]);
            registry.emplace<Components::Transform>(entity, transform);
        }
        if (node["Camera"])
        {
            auto camera = Components::DeserializeCamera(node["Camera"]);
            registry.emplace<Components::Camera>(entity, camera);
        }
        if (node["SphereCollider"])
        {
            auto sphereCollider = Components::DeserializeSphereCollider(node["SphereCollider"]);
            registry.emplace<Components::SphereCollider>(entity, sphereCollider);
        }
        if (node["Rigidbody"])
        {
            auto rigidbody = Components::DeserializeRigidbody(node["Rigidbody"]);
            registry.emplace<Components::Rigidbody>(entity, rigidbody);
        }
        if (node["Color"])
        {
            auto color = Components::DeserializeColor(node["Color"]);
            registry.emplace<Components::Color>(entity, color);
        }
        if (node["Intensity"])
        {
            auto intensity = Components::DeserializeIntensity(node["Intensity"]);
            registry.emplace<Components::Intensity>(entity, intensity);
        }
        if (node["LightFalloff"])
        {
            auto falloff = Components::DeserializeLightFalloff(node["LightFalloff"]);
            registry.emplace<Components::LightFalloff>(entity, falloff);
        }
        if (node["Spotlight"])
        {
            auto spotlight = Components::DeserializeSpotlight(node["Spotlight"]);
            registry.emplace<Components::Spotlight>(entity, spotlight);
        }
        if (node["Name"])
        {
            auto name = Components::DeserializeName(node["Name"]);
            registry.emplace<Components::Name>(entity, name);
        }

        // Tag Components
        if (node["BackgroundColor"])
        {
            registry.emplace<Components::BackgroundColor>(entity);
        }
        if (node["DirectionalLight"])
        {
            registry.emplace<Components::DirectionalLight>(entity);
        }
        if (node["PointLight"])
        {
            registry.emplace<Components::PointLight>(entity);
        }
    }

    void Scene::Deserialize(YAML::Node& node)
    {
        auto& registry = world.GetRegistry();
        for (auto entityNode : node)
        {
            const auto entity = registry.create();
            DeserializeComponents(registry, entity, entityNode);
        }
    }

    void Scene::Serialize(YAML::Emitter& emitter) const
    {
        const auto& registry = world.GetRegistry();
        const auto entities = registry.view<entt::entity>();
        emitter << YAML::BeginSeq;
        for (const auto entity : entities)
        {
            emitter << YAML::BeginMap;
            SerializeComponents(registry, entity, emitter);
            emitter << YAML::EndMap;
        }
        emitter << YAML::EndSeq;
    }

    Scene Scene::Copy() const
    {
        Scene newScene;
        YAML::Emitter emitter;
        Serialize(emitter);
        YAML::Node node = YAML::Load(emitter.c_str());
        newScene.Deserialize(node);

        return newScene;
    }
}