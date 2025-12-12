//
// Created by Oskar.Norberg on 2025-11-20.
//

#pragma once

#include <Assets/AssetLoader.h>

#include "Assets/AssetManager/AssetManager.h"
#include "Components/Components.h"
#include "Logging/Logger.h"
#include "UI/AssetDragAndDrop.h"
#include "entt/entity/registry.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

namespace RNGOEngine::Editor
{
    template<typename T>
    void DrawProperties(entt::registry& registry, const entt::entity entity);

    template<>
    inline void DrawProperties<Components::Name>(entt::registry& registry, const entt::entity entity)
    {
        // TODO: I don't like everything needing to check if the component exists.
        if (registry.any_of<Components::Name>(entity))
        {
            auto& name = registry.get<Components::Name>(entity);
            ImGui::InputText("Name", name.NameArr.data(), Components::MAX_NR_CHARACTERS);
        }
    }

    template<>
    inline void DrawProperties<Components::MeshRenderer>(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<Components::MeshRenderer>(entity))
        {
            ImGui::Text("MeshRenderer");
            auto& meshRenderer = registry.get<Components::MeshRenderer>(entity);

            auto modelMetadataOpt =
                AssetHandling::AssetDatabase::GetInstance().TryGetAssetMetadata(meshRenderer.ModelHandle);

            // Mesh Drag and Drop
            {
                const std::string filename =
                    modelMetadataOpt ? modelMetadataOpt->get().Path.filename().string() : "Missing";
                
                ImGui::PushID(&meshRenderer);

                ImGui::Selectable(filename.c_str());

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(AssetDragAndDropName))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(AssetDragAndDropPayload));

                        const auto& dragAndDropPayload =
                            *static_cast<const AssetDragAndDropPayload*>(payload->Data);

                        if (dragAndDropPayload.type != AssetHandling::AssetType::Model)
                        {
                            const auto badName = dragAndDropPayload.AssetPath.filename().string();
                            RNGO_LOG(
                                Core::LogLevel::Warning, "Cannot load asset {} as Model.", badName.c_str()
                            );
                        }
                        else
                        {
                            const auto newModelHandle = AssetHandling::AssetLoader::GetInstance().Load(
                                AssetHandling::AssetType::Model, dragAndDropPayload.AssetPath
                            );
                            meshRenderer.ModelHandle = newModelHandle;
                        }
                    }

                    ImGui::EndDragDropTarget();
                }

                ImGui::PopID();
            }

            if (ImGui::Button("Change Texture"))
            {
                ImGui::OpenPopup("TextureProperties");
            }

            if (ImGui::BeginPopupModal("TextureProperties"))
            {
                ImGui::Text("Texture Path");
                ImGui::Separator();

                static std::array<char, 256> texturePath{};
                ImGui::InputText("Path", texturePath.data(), 256);
                static int slot = 0;
                ImGui::InputInt("Slot", &slot);
                if (ImGui::Button("Load Texture"))
                {
                    const auto& materialHandle = meshRenderer.MaterialKey;

                    const auto textureHandle = AssetHandling::AssetLoader::GetInstance().Load(
                        AssetHandling::AssetType::Texture, texturePath.data()
                    );
                    AssetHandling::AssetManager::GetInstance().GetMaterialManager().SetTexture(
                        materialHandle, textureHandle, slot
                    );
                }

                if (ImGui::Button("Close"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    template<>
    inline void DrawProperties<Components::Transform>(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<Components::Transform>(entity))
        {
            auto& transform = registry.get<Components::Transform>(entity);

            ImGui::Text("Transform");
            ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.01f);

            glm::vec3 eulerRot = glm::degrees(glm::eulerAngles(transform.Rotation));
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(eulerRot), 0.01f))
            {
                transform.Rotation = glm::quat(glm::radians(eulerRot));
            }
            ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.01f, 0.0f);
        }
    }

    template<>
    inline void DrawProperties<Components::Camera>(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<Components::Camera>(entity))
        {
            ImGui::Text("Camera");

            auto& camera = registry.get<Components::Camera>(entity);
            ImGui::DragFloat("FOV", &camera.FOV, 0.1f, 0.1f, 180.0f);
        }
    }

    template<>
    inline void DrawProperties<Components::Color>(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<Components::Color>(entity))
        {
            ImGui::Text("Color");

            auto& color = registry.get<Components::Color>(entity);
            ImGui::DragFloat3("Color", glm::value_ptr(color.ColorValue), 0.01f, 0.0f, 1.0f);
        }
    }

    template<>
    inline void DrawProperties<Components::Intensity>(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<Components::Intensity>(entity))
        {
            ImGui::Text("Intensity");

            auto& intensity = registry.get<Components::Intensity>(entity);
            ImGui::DragFloat("Intensity", &intensity.IntensityValue, 0.01f);
        }
    }

    template<>
    inline void DrawProperties<Components::BackgroundColor>(
        entt::registry& registry, const entt::entity entity
    )
    {
        if (registry.any_of<Components::BackgroundColor>(entity))
        {
            ImGui::Text("BackgroundColor");
        }
    }

    template<>
    inline void DrawProperties<Components::LightFalloff>(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<Components::LightFalloff>(entity))
        {
            ImGui::Text("LightFalloff");

            auto& falloff = registry.get<Components::LightFalloff>(entity);
            ImGui::DragFloat("Constant", &falloff.Constant, 0.01f);
            ImGui::DragFloat("Linear", &falloff.Linear, 0.01f);
            ImGui::DragFloat("Quadratic", &falloff.Quadratic, 0.01f);
        }
    }

    template<>
    inline void DrawProperties<Components::AmbientLight>(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<Components::AmbientLight>(entity))
        {
            ImGui::Text("AmbientLight");
        }
    }

    template<>
    inline void DrawProperties<Components::DirectionalLight>(
        entt::registry& registry, const entt::entity entity
    )
    {
        if (registry.any_of<Components::DirectionalLight>(entity))
        {
            ImGui::Text("DirectionalLight");
        }
    }

    template<>
    inline void DrawProperties<Components::PointLight>(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<Components::PointLight>(entity))
        {
            ImGui::Text("PointLight");
        }
    }

    template<>
    inline void DrawProperties<Components::Spotlight>(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<Components::Spotlight>(entity))
        {
            ImGui::Text("Spotlight");

            auto& spotlight = registry.get<Components::Spotlight>(entity);
            ImGui::DragFloat("CutOff", &spotlight.CutOff, 0.01f);
            ImGui::DragFloat("OuterCutOff", &spotlight.OuterCutOff, 0.01f);
        }
    }
}
