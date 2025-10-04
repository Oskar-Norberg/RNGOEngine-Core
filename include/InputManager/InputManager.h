//
// Created by ringo on 2025-10-04.
//

#pragma once

#include <unordered_set>

#include "EventQueue/EventQueue.h"

namespace RNGOEngine::Core
{
    struct MouseDelta
    {
        double x, y;
    };

    class InputManager
    {
    public:
        void Update(const Events::EventQueue& eventQueue);

        // Mouse Movement
    public:
        MouseDelta GetMouseDelta() const;

        // Mouse Buttons
    public:
        bool IsMouseButtonDown(int button) const;
        bool WasMouseButtonPressedThisFrame(int button) const;
        bool WasMouseButtonReleasedThisFrame(int button) const;

        // Keyboard Keys
    public:
        bool IsKeyDown(int key) const;
        bool WasKeyPressedThisFrame(int key) const;
        bool WasKeyReleasedThisFrame(int key) const;

    private:
        std::unordered_set<int> m_currentlyPressedKeys;
        std::unordered_set<int> m_currentlyPressedMouseButtons;

        std::unordered_set<int> m_keysPressedThisFrame;
        std::unordered_set<int> m_keysReleasedThisFrame;

        std::unordered_set<int> m_mouseButtonsPressedThisFrame;
        std::unordered_set<int> m_mouseButtonsReleasedThisFrame;

    private:
        double m_deltaX = 0.0;
        double m_deltaY = 0.0;

    private:
        void UpdateKeyboard(const Events::EventQueue& eventQueue);
        void UpdateMousePresses(const Events::EventQueue& eventQueue);
        void UpdateMouseMovement(const Events::EventQueue& eventQueue);
    };
}