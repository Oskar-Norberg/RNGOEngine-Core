//
// Created by Oskar.Norberg on 2025-11-12.
//

#include "UI/Panels/StatsPanel.h"

#include "Utilities/Memory/MemoryCheckers.h"

namespace RNGOEngine::Editor
{
    void StatsPanel::Update(UIContext& context, const float deltaTime)
    {
        IDockablePanel::Update(context, deltaTime);

        AccumulateDeltaTime(deltaTime);
    }

    void StatsPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        RenderFPS(context);
        RenderStackUsage(context);
        RenderHeapUsage(context);
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
            RenderMemoryUsage(context, Utilities::Memory::GetStackMemoryUsage(), MemoryUnit::KB);
        }
    }

    void StatsPanel::RenderHeapUsage(UIContext& context)
    {
        if (ImGui::CollapsingHeader("Heap Usage"))
        {
            RenderMemoryUsage(context, Utilities::Memory::GetHeapMemoryUsage(), MemoryUnit::GB);
        }
    }
    void StatsPanel::RenderMemoryUsage(
        UIContext& context, Utilities::Memory::MemoryUsage memoryUsage, MemoryUnit unit
    )
    {
        const double percentUsed =
            static_cast<float>(memoryUsage.currentUsage) / static_cast<float>(memoryUsage.available) * 100.0f;

        size_t usage = memoryUsage.currentUsage;
        size_t total = memoryUsage.total;

        std::string_view unitView;
        switch (unit)
        {
            default:
            case MemoryUnit::Bytes:
                unitView = "B";
                break;
            case MemoryUnit::KB:
                unitView = "KB";
                usage /= 1024;
                total /= 1024;
                break;
            case MemoryUnit::MB:
                unitView = "MB";
                usage /= 1024 * 1024;
                total /= 1024 * 1024;
                break;
            case MemoryUnit::GB:
                unitView = "GB";
                usage /= 1024 * 1024 * 1024;
                total /= 1024 * 1024 * 1024;
                break;
        }

        ImGui::Text("Memory Usage: %u%s", usage, unitView.data());
        ImGui::Text("Size: %u%s", total, unitView.data());
        ImGui::Text("Memory Used: %.2f %%", percentUsed);
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