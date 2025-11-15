//
// Created by Oskar.Norberg on 2025-11-10.
//

#pragma once

#include <imgui.h>
#include <string_view>

#include "UIContext.h"

namespace RNGOEngine::Editor
{
    class IDockablePanel
    {
    public:
        virtual ~IDockablePanel() = default;

    public:
        virtual void Update(UIContext& context, float deltaTime)
        {
        }

        virtual void Render(UIContext& context)
        {
        }

    public:
        virtual void OnOpen(UIContext& context)
        {
        }

        virtual void OnClose(UIContext& context)
        {
        }

    public:
        void SetTargetHovered(UIContext& context, const bool status)
        {
            if (m_isHovered != status)
            {
                m_isHovered = status;
                if (m_isHovered)
                {
                    OnFocusGained(context);
                }
                else
                {
                    OnFocusLost(context);
                }
            }
        }

        virtual void OnFocusGained(UIContext& context)
        {
        }

        virtual void OnFocusLost(UIContext& context)
        {
        }

    public:
        virtual std::string_view GetPanelName() const = 0;

    private:
        bool m_isHovered = false;
    };
}