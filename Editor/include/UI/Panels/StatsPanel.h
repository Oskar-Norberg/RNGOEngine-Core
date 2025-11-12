//
// Created by Oskar.Norberg on 2025-11-12.
//

#pragma once

#include "UI/IDockablePanel.h"

namespace RNGOEngine::Editor
{
    class StatsPanel : public IDockablePanel
    {
    public:
        void Update(float deltaTime) override;
        void Render() override;

        std::string_view GetPanelName() const override
        {
            return "Stats";
        }

    private:
        float m_lastDeltaTime = 0.0f;

        static constexpr auto FPS_UPDATE_INTERVAL = 0.5f;
        float m_accumulatedDeltaTime = 0.0f;
        int m_frameCount = 0;
        float m_fps = 0.0f;
    };
}