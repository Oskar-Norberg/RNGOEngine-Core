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
        }

    public:
        virtual void OnOpen()
        {
        }

        virtual void OnClose()
        {
        }

    public:
        void SetTargetHovered(bool status)
        {
            if (m_isHovered != status)
            {
                m_isHovered = status;
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