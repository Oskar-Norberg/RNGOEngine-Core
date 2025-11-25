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

        for (const auto& logEntry : context.loggerSink->GetLogs())
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

            ImGui::Text("%s", logEntry.Message.c_str());

            if (pushedColor)
                ImGui::PopStyleColor();
        }
    }
}