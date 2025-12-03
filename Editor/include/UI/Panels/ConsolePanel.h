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
        Core::LogLevel m_filterLevel = Core::LogLevel::All;
        std::optional<size_t> m_selectedMessage;

        void DrawLogs(std::span<const Core::LogEntry> logs);
        void ScrollDownIfNeeded(size_t numberOfMessages);
        void DrawSelected(std::span<const Core::LogEntry> logs);

    private:
        void DrawFilterSelection();

    private:
        // TODO: Just return an owning string, its an editor panel it doesnt matter
        static std::string_view GetFormattedTime(const Core::LogEntry& entry);
        // TODO: Why does this return an owning-string? This is just a substring view??
        static std::string GetFromLastSlash(std::string_view string);
        static std::string GetFunctionName(const Core::LogEntry& entry);
    };
}