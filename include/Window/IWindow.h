//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <string_view>

#include "EventQueue/EventQueue.h"

namespace RNGOEngine::Core::Window
{
    class IWindow
    {
    public:
        virtual ~IWindow() = default;

    public:
        virtual void PollWindowEvents(Events::EventQueue& eventQueue) = 0;
        virtual void PollKeyboardEvents(Events::EventQueue& eventQueue) = 0;
        virtual void PollMouseEvents(Events::EventQueue& eventQueue) = 0;
        
        virtual void SwapBuffers() = 0;
        virtual void SetName(std::string_view name) = 0;

        /// 
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        /// 
        virtual bool ListenSendEvents(Events::EventQueue& eventQueue) = 0;
    };
}
