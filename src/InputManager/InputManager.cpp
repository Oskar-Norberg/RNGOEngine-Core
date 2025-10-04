//
// Created by ringo on 2025-10-04.
//

#include "InputManager/InputManager.h"

#include "EventQueue/EngineEvents/EngineEvents.h"

namespace RNGOEngine::Core
{
    void InputManager::Update(const Events::EventQueue& eventQueue)
    {
        UpdateKeyboard(eventQueue);
        UpdateMousePresses(eventQueue);
        UpdateMouseMovement(eventQueue);
    }

    void InputManager::UpdateKeyboard(const Events::EventQueue& eventQueue)
    {
        m_keysPressedThisFrame.clear();
        m_keysReleasedThisFrame.clear();

        const auto keyEvents = eventQueue.GetEvents<Events::KeyEvent>();
        for (const auto& [key, action] : keyEvents)
        {
            if (action == Events::ButtonAction::Press)
            {
                m_keysPressedThisFrame.insert(key);
            }
            else if (action == Events::ButtonAction::Release)
            {
                m_keysReleasedThisFrame.insert(key);
            }
        }

        for (const auto& key : m_keysPressedThisFrame)
        {
            m_currentlyPressedKeys.insert(key);
        }

        for (auto key_event : m_keysReleasedThisFrame)
        {
            m_currentlyPressedKeys.erase(key_event);
        }
    }

    void InputManager::UpdateMousePresses(const Events::EventQueue& eventQueue)
    {
        m_mouseButtonsPressedThisFrame.clear();
        m_mouseButtonsReleasedThisFrame.clear();

        const auto mouseButtonEvents = eventQueue.GetEvents<Events::MouseButtonEvent>();
        for (const auto& [key, action] : mouseButtonEvents)
        {
            if (action == Events::ButtonAction::Press)
            {
                m_mouseButtonsPressedThisFrame.insert(key);
            }
            else if (action == Events::ButtonAction::Release)
            {
                m_mouseButtonsReleasedThisFrame.insert(key);
            }
        }

        for (const auto& key : m_mouseButtonsPressedThisFrame)
        {
            m_currentlyPressedMouseButtons.insert(key);
        }

        for (auto key_event : m_mouseButtonsReleasedThisFrame)
        {
            m_currentlyPressedMouseButtons.erase(key_event);
        }
    }

    void InputManager::UpdateMouseMovement(const Events::EventQueue& eventQueue)
    {
        m_deltaX = 0.0;
        m_deltaY = 0.0;
        
        const auto mouseMoveEvents = eventQueue.GetEvents<Events::MouseMoveEvent>();
        for (const auto& [deltaX, deltaY] : mouseMoveEvents)
        {
            m_deltaX += deltaX;
            m_deltaY += deltaY;
        }
    }
}