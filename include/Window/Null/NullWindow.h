//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once
#include "Window/IWindow.h"

namespace RNGOEngine::Core::Window
{
    class NullWindow : public IWindow
    {
    public:
        void PollWindowEvents(Events::EventQueue& eventQueue) override
        {
        }

        void PollKeyboardEvents(Events::EventQueue& eventQueue) override
        {
        }

        void PollMouseEvents(Events::EventQueue& eventQueue) override
        {
        }

        void SwapBuffers() override
        {
        }

        void SetName(std::string_view name) override
        {
        }
    };
}