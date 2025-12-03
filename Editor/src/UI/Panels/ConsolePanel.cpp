//
// Created by Oskar.Norberg on 2025-11-25.
//

#include "UI/Panels/ConsolePanel.h"

#include "Logging/VectorSink.h"

namespace RNGOEngine::Editor
{
    void ConsolePanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        const auto logSpan = context.loggerSink->GetLogs();

        DrawLogs(logSpan);
        ScrollDownIfNeeded(logSpan.size());
    }

    void ConsolePanel::DrawLogs(const std::span<const Core::LogEntry> logs)
    {
        for (const auto& logEntry : logs)
        {
            bool pushedColor = false;

            switch (logEntry.Level)
            {
                case Core::LogLevel::Warning:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 0.0f, 1.0f));
                    pushedColor = true;
                    break;

                case Core::LogLevel::Error:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                    pushedColor = true;
                    break;

                case Core::LogLevel::Critical:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    pushedColor = true;
                    break;

                case Core::LogLevel::Debug:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                    pushedColor = true;
                    break;

                case Core::LogLevel::Info:
                default:
                    break;
            }

            ImGui::Text("[%s] %s", GetFormattedTime(logEntry).data(), logEntry.Message.c_str());

            if (pushedColor)
                ImGui::PopStyleColor();
        }
    }

    void ConsolePanel::ScrollDownIfNeeded(size_t numberOfMessages)
    {
        if (m_previousNrOfMessages != numberOfMessages)
        {
            ImGui::SetScrollHereY(1.0f);
            m_previousNrOfMessages = numberOfMessages;
        }
    }

    std::string_view ConsolePanel::GetFormattedTime(const Core::LogEntry& entry)
    {
        const auto time = std::chrono::system_clock::to_time_t(entry.TimePoint);

        // Three two-digit numbers + two colons + null terminator
        static std::array<char, 8 + 1> timeBuffer{};
        std::strftime(timeBuffer.data(), timeBuffer.size(), "%H:%M:%S", std::localtime(&time));

        return {timeBuffer.data(), timeBuffer.size()};
    }
}