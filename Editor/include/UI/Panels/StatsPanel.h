//
// Created by Oskar.Norberg on 2025-11-12.
//

#pragma once

#include "UI/IDockablePanel.h"
#include "Utilities/Memory/MemoryChecker.impl.h"

namespace RNGOEngine::Editor
{
    class StatsPanel : public IDockablePanel
    {
    public:
        void Update(UIContext& context, float deltaTime) override;
        void Render(UIContext& context) override;

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

    private:
        void RenderFPS(UIContext& context);
        void RenderStackUsage(UIContext& context);
        void RenderHeapUsage(UIContext& context);

    private:
        enum class MemoryUnit
        {
            Bytes,
            KB,
            MB,
            GB
        };
        void RenderMemoryUsage(
            UIContext& context, Utilities::Memory::MemoryUsage memoryUsage, MemoryUnit unit = MemoryUnit::KB
        );

    private:
        void AccumulateDeltaTime(float deltaTime);
    };
}