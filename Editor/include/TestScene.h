//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "Assets/AssetManager/AssetManager.h"
#include "Components/Components.h"
#include "Scene/Scene.h"

namespace RNGOEngine::Temporary
{
    // TODO: Only for testing. Remove later.
    class TestScene : public Core::Scene
    {
    public:
        void Initialize() override
        {
            using enum AssetHandling::AssetType;
            auto& assetManager = AssetHandling::AssetManager::GetInstance();
            auto& assetLoader = AssetHandling::AssetLoader::GetInstance();

            {
                // Cirno Setup
                const auto cirnoMesh = assetLoader.Load(Model, "cirno/cirno.obj");
                const auto cirnoTexture = assetLoader.Load(Texture, "cirno_albedo.jpeg");

                auto cirnoMaterial = assetManager.CreateMaterial("default.vert", "default.frag");
                cirnoMaterial.SetTexture(cirnoTexture, 0);

                const Components::Transform transform{
                    .position = glm::vec3(1.0f, -2.0f, -10.0f),
                    .rotation = {},
                    .scale = glm::vec3(1.0f)
                };

                auto cirnoEntity = world.CreateEntity({"Cirno"}, transform);
                cirnoEntity.AddComponent<Components::MeshRenderer>(
                    cirnoMesh, cirnoMaterial.GetMaterialAssetHandle()
                );
            }

            // Reimu setup
            {
                const auto reimuMesh = assetLoader.Load(Model, "reimu/reimu.obj");
                const auto reimuTexture = assetLoader.Load(Texture, "reimu_albedo.png");
                auto reimuMaterial = assetManager.CreateMaterial("default.vert", "default.frag");
                reimuMaterial.SetTexture(reimuTexture, 0);

                Components::Transform transform{
                    .position = glm::vec3(-1.0f, -2.0f, -10.0f),
                    .rotation = {},
                    .scale = glm::vec3(1.0f)
                };

                auto reimuEntity = world.CreateEntity({"Reimu"}, transform);
                reimuEntity.AddComponent<Components::MeshRenderer>(
                    reimuMesh, reimuMaterial.GetMaterialAssetHandle()
                );
            }

            // World Camera
            auto camera = world.CreateEntity({"Camera"});
            camera.AddComponent<Components::Camera>();

            // Ambient Light
            {
                auto ambientLight = world.CreateEntity();

                ambientLight.AddComponent<Components::AmbientLight>();
                ambientLight.AddComponent<Components::Color>();
                ambientLight.AddComponent<Components::Intensity>(0.1f);
            }

            // Directional Light
            {
                auto directionalLight = world.CreateEntity({"Directional Light"});

                directionalLight.AddComponent<Components::DirectionalLight>();
                directionalLight.AddComponent<Components::Color>();
                directionalLight.AddComponent<Components::Intensity>(0.1f);
            }

            // Point Light 1
            {
                Components::Transform transform{
                    .position = {-2.0f, 0.5f, -8.0f},
                    .rotation = glm::quat{}, .scale = {0.1f, 0.1f, 0.1f}
                };
                
                auto pointLight = world.CreateEntity({"Point Light"}, transform);

                const auto lampMesh = assetLoader.Load(Model, "sphere/sphere.fbx");
                const auto lampTexture = assetLoader.Load(Texture, "reimu_albedo.png");
                auto lampMaterial = assetManager.CreateMaterial("default.vert", "default.frag");
                lampMaterial.SetTexture(lampTexture, 0);



                pointLight.AddComponent<Components::PointLight>();
                pointLight.AddComponent<Components::LightFalloff>(0.5f, 0.2f, 0.007f);
                pointLight.AddComponent<Components::Color>(glm::vec3(0.25f, 0.25f, 1.0f));

                pointLight.AddComponent<Components::MeshRenderer>(
                    lampMesh, lampMaterial.GetMaterialAssetHandle()
                );
            }

            // Point Light 2
            {
                Components::Transform transform{
                    .position = {2.0f, 0.5f, -8.0f}, .rotation = glm::quat{}, .scale = {0.1f, 0.1f, 0.1f}
                };

                auto pointLight = world.CreateEntity({"Point Light"}, transform);

                const auto lampMesh = assetLoader.Load(Model, "sphere/sphere.fbx");
                const auto lampTexture = assetLoader.Load(Texture, "reimu_albedo.png");
                auto lampMaterial = assetManager.CreateMaterial("default.vert", "default.frag");
                lampMaterial.SetTexture(lampTexture, 0);

                pointLight.AddComponent<Components::PointLight>();
                pointLight.AddComponent<Components::LightFalloff>(0.5f, 0.2f, 0.007f);
                pointLight.AddComponent<Components::Color>(glm::vec3(1.0f, 0.25f, 0.25f));
                pointLight.AddComponent<Components::MeshRenderer>(
                    lampMesh, lampMaterial.GetMaterialAssetHandle()
                );
            }

            // Spotlight
            {
                Components::Transform transform{.scale = {0.1f, 0.1f, 0.1f}};

                auto spotlight = world.CreateEntity({"Spotlight"}, transform);

                spotlight.AddComponent<Components::Spotlight>();
                spotlight.AddComponent<Components::Intensity>(1.0f);
                spotlight.AddComponent<Components::LightFalloff>();
            }

            // Background Color
            {
                auto background = world.CreateEntity({"Background Color"});
                background.AddComponent<Components::BackgroundColor>();
                background.AddComponent<Components::Color>(glm::vec3(0.3f, 0.3f, 0.3f));
            }
        }

    private:
        static constexpr size_t ROWS = 40;
        static constexpr size_t COLS = 40;
        static constexpr float DISTANCE_BETWEEN = 2.5f;
    };
}