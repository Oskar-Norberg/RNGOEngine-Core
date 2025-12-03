//
// Created by Oskar.Norberg on 2025-11-25.
//

#pragma once

#include <span>

#include "Logging/Logger.h"
#include "UI/IDockablePanel.h"

namespace RNGOEngine::Editor
{
    class ConsolePanel : public IDockablePanel
    {
    public:
        void Render(UIContext& context) override;
        
        std::string_view GetPanelName() const override
        {
            return "Console";
        }

    private:
        size_t m_previousNrOfMessages = 0;

        void DrawLogs(std::span<const Core::LogEntry> logs);
        void ScrollDownIfNeeded(size_t numberOfMessages);

    private:
        static std::string_view GetFormattedTime(const Core::LogEntry& entry);
    };
}