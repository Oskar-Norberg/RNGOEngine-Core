//
// Created by Oskar.Norberg on 2025-11-25.
//

#pragma once

#include <span>

#include "Logging/Logger.h"
#include "UI/IDockablePanel.h"
#include "UI/PanelInitializer.h"

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
        Core::LogLevel m_filterLevel = Core::LogLevel::All & ~Core::LogLevel::Debug;
        std::optional<size_t> m_selectedMessage;

        void DrawLogWindow(std::span<const Core::LogEntry> logs);
        void DrawSelected(std::span<const Core::LogEntry> logs);

    private:
        void DrawFilterSelection();
        void ScrollDownIfNeeded(size_t numberOfMessages);
        void DrawLogs(std::span<const Core::LogEntry> logs);

    private:
        static std::string GetFormattedTime(const Core::LogEntry& entry);
        static std::string_view GetFromLastSlash(std::string_view string);
        static std::string GetFunctionName(const Core::LogEntry& entry);
    };
    EDITOR_PANEL(ConsolePanel);
}