//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/Panels/HierarchyPanel.h"

#include "Components/Components.h"
#include "Scene/SceneManager/SceneManager.h"
#include "glm/gtc/type_ptr.inl"

namespace RNGOEngine::Editor
{
    HierarchyPanel::HierarchyPanel(Core::SceneManager& sceneManager)
        : m_sceneManager(sceneManager)
    {
    }

    void HierarchyPanel::Render()
    {
        IDockablePanel::Render();

        auto* world = m_sceneManager.GetCurrentWorld();
        if (!world)
        {
            ImGui::Text("No world loaded.");
            return;
        }

        auto& registry = world->GetRegistry();
        ImGui::Text("Entities in World:");

        const auto entities = registry.view<entt::entity>();
        for (auto entity : entities)
        {
            auto id = static_cast<uint32_t>(entity);
            if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<uintptr_t>(entity)), "Entity %u", id))
            {
                // TODO:Temporarily display components directly in here. Later this will need a separate panel / proper serialization.
                if (registry.any_of<Components::MeshRenderer>(entity))
                {
                    ImGui::Text("MeshRenderer");
                    const auto& meshRenderer = registry.get<Components::MeshRenderer>(entity);
                    ImGui::Text("ModelHandle %u", meshRenderer.modelHandle.GetValue());
                    ImGui::Text("MaterialHandle %u", meshRenderer.materialKey.GetValue());
                }

                if (registry.any_of<Components::Transform>(entity))
                {
                    if (ImGui::TreeNode("Transform"))
                    {
                        auto& transform = registry.get<Components::Transform>(entity);
                        ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.01f);
                        glm::vec3 eulerRot = glm::degrees(glm::eulerAngles(transform.rotation));
                        if (ImGui::DragFloat3("Rotation", glm::value_ptr(eulerRot), 0.01f))
                        {
                            transform.rotation = glm::quat(glm::radians(eulerRot));
                        }
                        ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.01f, 0.0f);
                        ImGui::TreePop();
                    }
                }

                if (registry.any_of<Components::Camera>(entity))
                {
                    if (ImGui::TreeNode("Camera"))
                    {
                        auto& camera = registry.get<Components::Camera>(entity);
                        ImGui::DragFloat("FOV", &camera.fov, 0.1f, 0.1f, 180.0f);
                        ImGui::TreePop();
                    }
                }

                if (registry.any_of<Components::Color>(entity))
                {
                    if (ImGui::TreeNode("Color"))
                    {
                        auto& color = registry.get<Components::Color>(entity);
                        ImGui::DragFloat3("Color", glm::value_ptr(color.color), 0.01f, 0.0f, 1.0f);
                        ImGui::TreePop();
                    }
                }

                if (registry.any_of<Components::Intensity>(entity))
                {
                    if (ImGui::TreeNode("Intensity"))
                    {
                        auto& intensity = registry.get<Components::Intensity>(entity);
                        ImGui::DragFloat("Intensity", &intensity.intensity, 0.01f);
                        ImGui::TreePop();
                    }
                }

                if (registry.any_of<Components::BackgroundColor>(entity))
                {
                    ImGui::Text("BackgroundColor");
                }

                if (registry.any_of<Components::LightFalloff>(entity))
                {
                    if (ImGui::TreeNode("LightFalloff"))
                    {
                        auto& falloff = registry.get<Components::LightFalloff>(entity);
                        ImGui::DragFloat("Constant", &falloff.constant, 0.01f);
                        ImGui::DragFloat("Linear", &falloff.linear, 0.01f);
                        ImGui::DragFloat("Quadratic", &falloff.quadratic, 0.01f);
                        ImGui::TreePop();
                    }
                }

                if (registry.any_of<Components::AmbientLight>(entity))
                {
                    ImGui::Text("AmbientLight");
                }

                if (registry.any_of<Components::DirectionalLight>(entity))
                {
                    ImGui::Text("DirectionalLight");
                }

                if (registry.any_of<Components::PointLight>(entity))
                {
                    ImGui::Text("PointLight");
                }

                if (registry.any_of<Components::Spotlight>(entity))
                {
                    if (ImGui::TreeNode("Spotlight"))
                    {
                        auto& spotlight = registry.get<Components::Spotlight>(entity);
                        ImGui::DragFloat("CutOff", &spotlight.cutOff, 0.01f);
                        ImGui::DragFloat("OuterCutOff", &spotlight.outerCutOff, 0.01f);
                        ImGui::TreePop();
                    }
                }

                ImGui::TreePop();
            }
        }
    }
}