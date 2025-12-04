//
// Created by Oskar.Norberg on 2025-11-12.
//

#include "UI/Panels/StatsPanel.h"

#include "Utilities/Memory/MemoryCheckers.h"

namespace RNGOEngine::Editor
{
    void StatsPanel::Update(UIContext& context, float deltaTime)
    {
        IDockablePanel::Update(context, deltaTime);

        AccumulateDeltaTime(deltaTime);
    }

    void StatsPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        RenderFPS(context);
        RenderStackUsage(context);
    }

    void StatsPanel::RenderFPS(UIContext& context)
    {
        ImGui::Text("FPS: %.2f", m_fps);
        ImGui::Text("Frame Time: %.2f ms", m_lastDeltaTime * 1000.0f);
    }

    void StatsPanel::RenderStackUsage(UIContext& context)
    {
        if (ImGui::CollapsingHeader("Stack Usage"))
        {
            const auto memory = Utilities::Memory::GetStackMemoryUsage();
            const double percentUsed =
                static_cast<float>(memory.currentUsage) / static_cast<float>(memory.available) * 100.0f;

            const size_t usageKB = memory.currentUsage / 1024;
            const size_t totalKB = memory.total / 1024;
            ImGui::Text("Stack Memory Usage: %uKB", usageKB);
            ImGui::Text("Stack Size: %uKB", totalKB);
            ImGui::Text("Stack Memory Used: %.2f %%", percentUsed);
        }
    }

    void StatsPanel::AccumulateDeltaTime(float deltaTime)
    {
        m_lastDeltaTime = deltaTime;

        // Accumulate FPS
        m_accumulatedDeltaTime += deltaTime;
        m_frameCount++;
        if (m_accumulatedDeltaTime >= FPS_UPDATE_INTERVAL)
        {
            m_fps = static_cast<float>(m_frameCount) / m_accumulatedDeltaTime;
            m_frameCount = 0;
            m_accumulatedDeltaTime = 0.0f;
        }
    }
}