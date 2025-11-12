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
                const auto cirnoMesh = assetLoader.Load(Model, "cirno/scene.gltf");
                const auto cirnoTexture = assetLoader.Load(Texture, "cirno_albedo.jpeg");

                auto cirnoMaterial = assetManager.CreateMaterial("default.vert", "default.frag");
                cirnoMaterial.SetTexture(cirnoTexture, 0);

                const auto cirnoEntity = world.CreateEntity();
                world.GetRegistry().emplace<Components::MeshRenderer>(
                    cirnoEntity, cirnoMesh, cirnoMaterial.GetMaterialAssetHandle()
                );
                world.GetRegistry().emplace<Components::Transform>(
                    cirnoEntity,
                    Components::Transform{
                        .position = glm::vec3(1.0f, 1.0f, -10.0f),
                        .rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
                        .scale = glm::vec3(.1f)
                    }
                );
            }

            // Reimu setup
            {
                const auto reimuMesh = assetLoader.Load(Model, "reimu/scene.gltf");
                const auto reimuTexture = assetLoader.Load(Texture, "reimu_albedo.png");
                auto reimuMaterial = assetManager.CreateMaterial("default.vert", "default.frag");
                reimuMaterial.SetTexture(reimuTexture, 0);

                const auto reimuEntity = world.CreateEntity();
                world.GetRegistry().emplace<Components::MeshRenderer>(
                    reimuEntity, reimuMesh, reimuMaterial.GetMaterialAssetHandle()
                );
                world.GetRegistry().emplace<Components::Transform>(
                    reimuEntity,
                    Components::Transform{
                        .position = glm::vec3(-1.0f, 1.0f, -10.0f),
                        .rotation = glm::angleAxis(glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
                        .scale = glm::vec3(.1f)
                    }
                );
            }

            // World Camera
            const auto camera = world.CreateEntity();
            world.GetRegistry().emplace<Components::Camera>(camera);
            world.GetRegistry().emplace<Components::Transform>(
                camera,
                Components::Transform{
                    // .position = {ROWS * DISTANCE_BETWEEN / 2.0f, 7.5f, COLS * DISTANCE_BETWEEN / 2.0f},
                    .position = glm::vec3(0.0f),
                    .rotation = glm::quat{},
                    .scale = {1.0f, 1.0f, 1.0f}
                }
            );

            // Ambient Light
            {
                const auto ambientLight = world.CreateEntity();
                world.GetRegistry().emplace<Components::AmbientLight>(ambientLight);
                world.GetRegistry().emplace<Components::Color>(ambientLight);
                world.GetRegistry().emplace<Components::Intensity>(ambientLight, 0.1f);
            }

            // Directional Light
            {
                const auto directionalLight = world.CreateEntity();
                world.GetRegistry().emplace<Components::DirectionalLight>(directionalLight);
                world.GetRegistry().emplace<Components::Transform>(directionalLight);
                world.GetRegistry().emplace<Components::Intensity>(directionalLight, 0.1f);
                world.GetRegistry().emplace<Components::Color>(directionalLight);
            }

            // Point Light 1
            {
                const auto pointLight = world.CreateEntity();
                
                const auto lampMesh = assetLoader.Load(Model, "sphere/sphere.fbx");
                const auto lampTexture = assetLoader.Load(Texture, "reimu_albedo.png");
                auto lampMaterial = assetManager.CreateMaterial("default.vert", "default.frag");
                lampMaterial.SetTexture(lampTexture, 0);

                world.GetRegistry().emplace<Components::PointLight>(pointLight);
                world.GetRegistry().emplace<Components::Transform>(
                    pointLight,
                    Components::Transform{
                        .position = {-2.0f, -1.7f, -8.0f},
                        .rotation = glm::quat{},
                        .scale = {0.1f, 0.1f, 0.1f}
                    }
                );
                world.GetRegistry().emplace<Components::LightFalloff>(pointLight,
                    0.5f, 0.2f, 0.007f);
                world.GetRegistry().emplace<Components::Color>(pointLight,
                glm::vec3(0.25f, 0.25f, 1.0f)
                    );
                world.GetRegistry().emplace<Components::MeshRenderer>(
                    pointLight, lampMesh, lampMaterial.GetMaterialAssetHandle()
                );
            }

            // Point Light 2
            {
                const auto pointLight = world.CreateEntity();
                
                const auto lampMesh = assetLoader.Load(Model, "sphere/sphere.fbx");
                const auto lampTexture = assetLoader.Load(Texture, "reimu_albedo.png");
                auto lampMaterial = assetManager.CreateMaterial("default.vert", "default.frag");
                lampMaterial.SetTexture(lampTexture, 0);

                world.GetRegistry().emplace<Components::PointLight>(pointLight);
                world.GetRegistry().emplace<Components::Transform>(
                    pointLight,
                    Components::Transform{
                        .position = {2.0f, -1.7f, -8.0f},
                        .rotation = glm::quat{},
                        .scale = {0.1f, 0.1f, 0.1f}
                    }
                );
                world.GetRegistry().emplace<Components::LightFalloff>(pointLight,
                    0.5f, 0.2f, 0.007f);
                world.GetRegistry().emplace<Components::Color>(pointLight,
                glm::vec3(1.0f, 0.25f, 0.25f)
                    );
                world.GetRegistry().emplace<Components::MeshRenderer>(
                    pointLight, lampMesh, lampMaterial.GetMaterialAssetHandle()
                );
            }

            // Spotlight
            {
                const auto spotlight = world.CreateEntity();
                world.GetRegistry().emplace<Components::Spotlight>(spotlight);
                world.GetRegistry().emplace<Components::Intensity>(spotlight, 1.0f);
                world.GetRegistry().emplace<Components::LightFalloff>(spotlight);
                world.GetRegistry().emplace<Components::Transform>(
                    spotlight, Components::Transform{.scale = {0.1f, 0.1f, 0.1f}}
                );
            }

            // Background Color
            {
                const auto background = world.CreateEntity();
                world.GetRegistry().emplace<Components::BackgroundColor>(background);
                world.GetRegistry().emplace<Components::Color>(
                    background, glm::vec3(0.3f, 0.3f, 0.3f));
            }
        }

    private:
        static constexpr size_t ROWS = 40;
        static constexpr size_t COLS = 40;
        static constexpr float DISTANCE_BETWEEN = 2.5f;
    };
}