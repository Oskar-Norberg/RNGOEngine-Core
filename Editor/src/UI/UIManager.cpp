//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/UIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ImGuizmo.h"
#include "Window/IWindow.h"

namespace RNGOEngine::Editor
{
    UIManager::UIManager(Core::Window::IWindow& window, Core::SceneManager& sceneManager)
        : m_window(window)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // IF using Docking Branch

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window.GetNativeWindow()), true);
        // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();
    }
    void UIManager::Update(UIContext& context, float deltaTime)
    {
        for (const auto& panel : m_panels)
        {
            panel->Update(context, deltaTime);
        }
    }

    void UIManager::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        ImGui::DockSpaceOverViewport();
    }
    void UIManager::Render(UIContext& context)
    {
        for (const auto& panel : m_panels)
        {
            ImGui::Begin(panel->GetPanelName().data());

            {
                const bool hovered = ImGui::IsWindowHovered();
                panel->SetTargetHovered(context, hovered);
            }

            panel->Render(context);
            ImGui::End();
        }
    }

    void UIManager::EndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}