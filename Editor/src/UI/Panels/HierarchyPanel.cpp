//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/Panels/HierarchyPanel.h"

#include "Components/Components.h"
#include "Scene/SceneManager/SceneManager.h"
#include "UI/Managers/UISelectionManager.h"
#include "glm/gtc/type_ptr.inl"

namespace RNGOEngine::Editor
{
    void HierarchyPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        auto* world = context.sceneManager->GetCurrentWorld();
        if (!world)
        {
            ImGui::Text("No world loaded.");
            return;
        }

        auto& registry = world->GetRegistry();

        if (ImGui::Button("Create Entity"))
        {
            ImGui::OpenPopup("EntityCreationPopup");
        }

        if (ImGui::BeginPopup("EntityCreationPopup"))
        {
            if (ImGui::Button("Background Color"))
            {
                CreateBackgroundColorEntity(context);
            }

            if (ImGui::Button("Camera"))
            {
                CreateCameraEntity(context);
            }

            if (ImGui::Button("Ambient Light"))
            {
                CreateAmbientLightEntity(context);
            }

            if (ImGui::Button("Directional Light"))
            {
                CreateDirectionalLightEntity(context);
            }

            if (ImGui::Button("Point Light"))
            {
                CreatePointLightEntity(context);
            }

            if (ImGui::Button("Spotlight"))
            {
                CreateSpotlightEntity(context);
            }

            if (ImGui::Button("Mesh"))
            {
                CreateMeshEntity(context);
            }

            ImGui::EndPopup();
        }

        const auto entities = registry.view<entt::entity>();
        for (auto entity : entities)
        {
            ImGui::PushID(reinterpret_cast<void*>(static_cast<std::uintptr_t>(entity)));

            std::string_view name = registry.any_of<Components::Name>(entity)
                                        ? registry.get<Components::Name>(entity).NameArr.data()
                                        : "Unnamed Entity";

            if (name.empty())
            {
                name = "Unnamed Entity";
            }

            if (ImGui::Selectable(name.data()))
            {
                context.selectionManager.SetSelectedEntity(entity);
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup("EntityOptionsPopup");
            }

            if (ImGui::BeginPopup("EntityOptionsPopup"))
            {
                if (ImGui::Button("Destroy"))
                {
                    registry.destroy(entity);
                }

                if (ImGui::Button("Duplicate"))
                {
                    const auto newEntity = registry.create();
                    for (auto [id, storage] : registry.storage())
                    {
                        if (storage.contains(entity))
                        {
                            storage.push(newEntity, storage.value(entity));
                        }
                    }

                    if (registry.any_of<Components::Name>(newEntity))
                    {
                        auto& nameComp = registry.get<Components::Name>(newEntity);
                        std::string newName = std::string(nameComp.NameArr.data()) + " (Copy)";
                        std::snprintf(
                            nameComp.NameArr.data(), Components::MAX_NR_CHARACTERS, "%s", newName.c_str()
                        );
                    }
                }

                ImGui::EndPopup();
            }

            ImGui::PopID();
        }
    }

    void HierarchyPanel::CreateBackgroundColorEntity(UIContext& context)
    {
        constexpr auto nameComponent = Components::Name{"Background Color"};
        auto entity = context.sceneManager->GetCurrentWorld()->CreateEntity(nameComponent);
        entity.AddComponent<Components::BackgroundColor>();
        entity.AddComponent<Components::Color>();
    }
    void HierarchyPanel::CreateCameraEntity(UIContext& context)
    {
        // TODO: Again, having to wrap it in a name component from call-site is horrible.
        constexpr auto nameComponent = Components::Name{"Camera"};
        auto entity = context.sceneManager->GetCurrentWorld()->CreateEntity(nameComponent);
        entity.AddComponent<Components::Camera>();
    }

    void HierarchyPanel::CreateAmbientLightEntity(UIContext& context)
    {
        constexpr auto nameComponent = Components::Name{"Ambient Light"};
        auto entity = context.sceneManager->GetCurrentWorld()->CreateEntity(nameComponent);

        entity.AddComponent<Components::AmbientLight>();
        entity.AddComponent<Components::Color>();
        entity.AddComponent<Components::Intensity>(0.2f);
    }

    void HierarchyPanel::CreateDirectionalLightEntity(UIContext& context)
    {
        constexpr auto nameComponent = Components::Name{"Directional Light"};
        auto transform = Components::Transform{
            .Position = glm::vec3(0.0f),
            .Rotation = glm::quat(glm::radians(glm::vec3(-45.0f, -45.0f, 0.0f))),
            .Scale = glm::vec3(1.0f)
        };
        auto entity = context.sceneManager->GetCurrentWorld()->CreateEntity(nameComponent, transform);

        entity.AddComponent<Components::DirectionalLight>();
        entity.AddComponent<Components::Color>();
        entity.AddComponent<Components::Intensity>(0.75f);
    }

    void HierarchyPanel::CreatePointLightEntity(UIContext& context)
    {
        constexpr auto nameComponent = Components::Name{"Point Light"};
        auto entity = context.sceneManager->GetCurrentWorld()->CreateEntity(nameComponent);

        entity.AddComponent<Components::PointLight>();
        entity.AddComponent<Components::Color>();
        entity.AddComponent<Components::Intensity>(0.8f);
        entity.AddComponent<Components::LightFalloff>();
    }

    void HierarchyPanel::CreateSpotlightEntity(UIContext& context)
    {
        constexpr auto nameComponent = Components::Name{"Spotlight"};
        auto entity = context.sceneManager->GetCurrentWorld()->CreateEntity(nameComponent);

        entity.AddComponent<Components::Spotlight>();
        entity.AddComponent<Components::Color>();
        entity.AddComponent<Components::Intensity>(0.8f);
        entity.AddComponent<Components::LightFalloff>();
    }

    void HierarchyPanel::CreateMeshEntity(UIContext& context)
    {
        constexpr auto nameComponent = Components::Name{"Mesh"};
        auto entity = context.sceneManager->GetCurrentWorld()->CreateEntity(nameComponent);

        entity.AddComponent<Components::MeshRenderer>();
    }
}