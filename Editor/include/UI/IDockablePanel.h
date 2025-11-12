//
// Created by Oskar.Norberg on 2025-11-10.
//

#pragma once

#include <imgui.h>

#include <string_view>

namespace RNGOEngine::Editor
{
    class IDockablePanel
    {
    public:
        virtual ~IDockablePanel() = default;

    public:
        virtual void Update(float deltaTime)
        {
        }

        virtual void Render()
        {
            if (const bool newHovered = ImGui::IsWindowHovered(); newHovered != m_isHovered)
            {
                m_isHovered = newHovered;

                if (m_isHovered)
                {
                    OnFocusGained();
                }
                else
                {
                    OnFocusLost();
                }
            }
        }

    public:
        virtual void OnOpen()
        {
        }

        virtual void OnClose()
        {
        }

    public:
        virtual void OnFocusGained()
        {
        }

        virtual void OnFocusLost()
        {
        }

    public:
        virtual std::string_view GetPanelName() const = 0;

    private:
        bool m_isHovered = false;
    };
}