//
// Created by Oskar.Norberg on 2025-11-12.
//

#include "UI/Panels/StatsPanel.h"

namespace RNGOEngine::Editor
{
    void StatsPanel::Update(UIContext& context, float deltaTime)
    {
        IDockablePanel::Update(context, deltaTime);

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

    void StatsPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        ImGui::Text("FPS: %.2f", m_fps);
        ImGui::Text("Frame Time: %.2f ms", m_lastDeltaTime * 1000.0f);
    }
}