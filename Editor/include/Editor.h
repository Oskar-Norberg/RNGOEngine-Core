//
// Created by Oskar.Norberg on 2025-11-05.
//

#pragma once
#include "Engine.h"

namespace RNGOEngine::Editor
{
    class Editor
    {
    public:
        Editor();

        void Run();

    private:
        std::unique_ptr<Core::Engine> m_engine;
    };
}