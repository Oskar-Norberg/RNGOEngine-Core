//
// Created by Oskar.Norberg on 2025-11-05.
//

#pragma once

#include <Application/Application.h>

#include "UI/UIManager.h"

namespace RNGOEngine::Editor
{
    class Editor : public Application
    {
    public:
        explicit Editor(const EngineConfig& config);

    public:
        void OnUpdate(float deltaTime) override;
        void OnRender() override;

    private:
        UIManager m_UIManager;
        
    private:
        void UpdateEngineSystems(float deltaTime);
        void UpdateGameSystems(float deltaTime);
    };
}