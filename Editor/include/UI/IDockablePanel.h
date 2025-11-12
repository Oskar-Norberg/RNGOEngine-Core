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
        virtual void OnFocusGained()
        {
        }

        virtual void OnFocusLost()
        {
        }

    public:
        virtual void OnResize(const int width, const int height)
        {
            m_width = width;
            m_height = height;
        }

    public:
        virtual std::string_view GetPanelName() const = 0;

    protected:
        int m_width = 0, m_height = 0;
    };
}