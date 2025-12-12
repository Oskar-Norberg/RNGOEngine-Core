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

        const ImVec2 availableSize = ImGui::GetContentRegionAvail();
        const float availableX = availableSize.x;
        const ImVec2 spacing = ImGui::GetStyle().ItemSpacing;

        const float logY =
            m_selectedMessage ? static_cast<int>(availableSize.y * 0.6) - spacing.y * 0.5f : availableSize.y;
        const float selectedY = static_cast<int>(availableSize.y * 0.4) - spacing.y * 0.5f;

        {
            ImGui::BeginChild("Log", ImVec2(availableX, logY));
            DrawLogWindow(logSpan);
            ImGui::EndChild();
        }

        if (m_selectedMessage)
        {
            ImGui::BeginChild("Selected", ImVec2(availableX, selectedY));
            DrawSelected(logSpan);
            ImGui::EndChild();
        }
    }

    void ConsolePanel::DrawLogWindow(const std::span<const Core::LogEntry> logs)
    {
        DrawFilterSelection();
        ImGui::Separator();
        {
            ImGui::BeginChild("LogList");
            DrawLogs(logs);
            ImGui::EndChild();
        }
    }

    void ConsolePanel::ScrollDownIfNeeded(const size_t numberOfMessages)
    {
        if (m_previousNrOfMessages != numberOfMessages)
        {
            ImGui::SetScrollHereY(1.0f);
            m_previousNrOfMessages = numberOfMessages;
        }
    }
    void ConsolePanel::DrawLogs(const std::span<const Core::LogEntry> logs)
    {
        constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg;
        if (ImGui::BeginTable("LogTable", 1, tableFlags))
        {
            for (size_t i = 0; i < logs.size(); i++)
            {
                const auto& logEntry = logs[i];

                if ((logEntry.Level & m_filterLevel) == Core::LogLevel::None)
                {
                    continue;
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                bool pushedLogColor = false;

                switch (logEntry.Level)
                {
                    case Core::LogLevel::Warning:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 0.0f, 1.0f));
                        pushedLogColor = true;
                        break;

                    case Core::LogLevel::Error:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                        pushedLogColor = true;
                        break;

                    case Core::LogLevel::Critical:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                        pushedLogColor = true;
                        break;

                    case Core::LogLevel::Info:
                    default:
                        break;
                }

                ImGui::Text("[%s] %s", GetFormattedTime(logEntry).data(), logEntry.Message.c_str());

                if (pushedLogColor)
                {
                    ImGui::PopStyleColor();
                }

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly))
                {
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(60, 60, 60, 255));

                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemActive())
                    {
                        m_selectedMessage = i;
                    }
                }
            }

            ImGui::EndTable();
        }

        ScrollDownIfNeeded(logs.size());
    }

    void ConsolePanel::DrawSelected(std::span<const Core::LogEntry> logs)
    {
        if (!m_selectedMessage)
        {
            return;
        }

        auto selectedMessageIndex = m_selectedMessage.value();

        // LogEntry has been devalidated. Deselect.
        if (logs.size() <= selectedMessageIndex)
        {
            m_selectedMessage = std::nullopt;
            return;
        }

        auto& selectedMessage = logs[selectedMessageIndex];

        ImGui::Separator();

        if (ImGui::Button("X"))
        {
            m_selectedMessage.reset();
        }

        ImGui::SameLine();

        ImGui::Text(
            "%s:%s:%d", GetFromLastSlash(selectedMessage.Location.File.data()).data(),
            GetFunctionName(selectedMessage).data(), selectedMessage.Location.Line
        );
        ImGui::TextWrapped(
            "[%s] %s", magic_enum::enum_name(selectedMessage.Level).data(), selectedMessage.Message.c_str()
        );
    }
    void ConsolePanel::DrawFilterSelection()
    {
        if (ImGui::BeginCombo("##LogLevelCombo", "Filter"))
        {
            constexpr auto nrOfLogLevels = magic_enum::enum_count<Core::LogLevel>();
            // Skip first "None" and last "All"
            for (size_t i = 1; i < nrOfLogLevels - 1; ++i)
            {
                const auto logLevel = magic_enum::enum_value<Core::LogLevel>(i);
                const auto levelName = magic_enum::enum_name(logLevel);
                bool isSelected = (m_filterLevel & logLevel) == logLevel;
                if (ImGui::Selectable(levelName.data(), &isSelected))
                {
                    if (!isSelected)
                    {
                        m_filterLevel &= ~logLevel;
                    }
                    else
                    {
                        m_filterLevel |= logLevel;
                    }
                }
            }

            ImGui::EndCombo();
        }
    }

    std::string ConsolePanel::GetFormattedTime(const Core::LogEntry& entry)
    {
        const auto time = std::chrono::system_clock::to_time_t(entry.TimePoint);

        // Three two-digit numbers + two colons + null terminator
        std::string timeBuffer(8 + 1, '\0');
        std::strftime(timeBuffer.data(), timeBuffer.size(), "%H:%M:%S", std::localtime(&time));

        return timeBuffer;
    }

    std::string_view ConsolePanel::GetFromLastSlash(std::string_view string)
    {
        const auto lastSlash = string.find_last_of("/\\");

        if (lastSlash == std::string::npos)
        {
            return {};
        }

        return {string.begin() + lastSlash + 1, string.end()};
    }

    std::string ConsolePanel::GetFunctionName(const Core::LogEntry& entry)
    {
        auto& function = entry.Location.Function;
        const size_t funcNameEndIndex = function.find_last_of("(");

        if (funcNameEndIndex == std::string::npos)
        {
            return {};
        }

        const std::string_view untilNameEndView = function.substr(0, funcNameEndIndex);
        const size_t funcNameStartIndex = untilNameEndView.find_last_of("::");

        if (funcNameStartIndex == std::string::npos)
        {
            return {};
        }

        return std::string(
            function.begin() + funcNameStartIndex + 1, function.end() - (function.size() - funcNameEndIndex)
        );
    }
}