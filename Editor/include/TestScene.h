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

            const auto cirnoMesh = assetLoader.Load(Model, "cirno_fumo/cirno_low.obj");
            const auto sphereMesh = assetLoader.Load(Model, "sphere/sphere.fbx");

            // const auto backpackTexture = assetManager.LoadTexture("backpack.jpg");
            const auto cirnoTexture = assetLoader.Load(Texture, "cirno.jpg");

            auto cirnoMaterial = assetManager.CreateMaterial("default.vert", "default.frag");
            cirnoMaterial.SetTexture(cirnoTexture, 0);

            for (size_t x = 0; x < ROWS; x++)
            {
                for (size_t y = 0; y < COLS; y++)
                {
                    const auto box = world.CreateEntity();

                    world.GetRegistry().emplace<Components::MeshRenderer>(
                        box, cirnoMesh, cirnoMaterial.GetMaterialAssetHandle()
                    );

                    Components::Transform transform{
                        .position = glm::vec3(x * DISTANCE_BETWEEN, 0.0f, y * DISTANCE_BETWEEN),
                        .rotation = glm::quat{},
                        .scale = glm::vec3(0.1f)
                    };

                    transform.rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                    world.GetRegistry().emplace<Components::Transform>(box, transform);
                }
            }

            const auto camera = world.CreateEntity();
            world.GetRegistry().emplace<Components::Camera>(camera);
            world.GetRegistry().emplace<Components::Transform>(
                camera,
                Components::Transform{
                    .position = {ROWS * DISTANCE_BETWEEN / 2.0f, 7.5f, COLS * DISTANCE_BETWEEN / 2.0f},
                    .rotation = glm::quat{},
                    .scale = {1.0f, 1.0f, 1.0f}
                }
            );

            const auto ambientLight = world.CreateEntity();
            world.GetRegistry().emplace<Components::AmbientLight>(ambientLight);
            world.GetRegistry().emplace<Components::Color>(ambientLight);
            world.GetRegistry().emplace<Components::Intensity>(ambientLight, 0.1f);

            const auto directionalLight = world.CreateEntity();
            world.GetRegistry().emplace<Components::DirectionalLight>(directionalLight);
            world.GetRegistry().emplace<Components::Transform>(directionalLight);
            world.GetRegistry().emplace<Components::Intensity>(directionalLight, 0.1f);
            world.GetRegistry().emplace<Components::Color>(directionalLight);

            const auto pointLight = world.CreateEntity();
            
            world.GetRegistry().emplace<Components::PointLight>(pointLight);
            world.GetRegistry().emplace<Components::Transform>(
                pointLight,
                Components::Transform{
                    .position = {ROWS * DISTANCE_BETWEEN / 2.0f, 7.5f, COLS * DISTANCE_BETWEEN / 2.0f},
                    .rotation = glm::quat{},
                    .scale = {0.1f, 0.1f, 0.1f}
                }
            );
            world.GetRegistry().emplace<Components::LightFalloff>(pointLight);
            world.GetRegistry().emplace<Components::MeshRenderer>(
                pointLight, sphereMesh, cirnoMaterial.GetMaterialAssetHandle()
            );

            const auto spotlight = world.CreateEntity();
            world.GetRegistry().emplace<Components::Spotlight>(spotlight);
            world.GetRegistry().emplace<Components::Intensity>(spotlight, 1.0f);
            world.GetRegistry().emplace<Components::LightFalloff>(spotlight);
            world.GetRegistry().emplace<Components::Transform>(
                spotlight, Components::Transform{.scale = {0.1f, 0.1f, 0.1f}}
            );

            const auto background = world.CreateEntity();
            world.GetRegistry().emplace<Components::BackgroundColor>(background);
            world.GetRegistry().emplace<Components::Color>(
                background, glm::vec3(0.3f, 0.3f, 0.3f));
        }

    private:
        static constexpr size_t ROWS = 40;
        static constexpr size_t COLS = 40;
        static constexpr float DISTANCE_BETWEEN = 2.5f;
    };
}