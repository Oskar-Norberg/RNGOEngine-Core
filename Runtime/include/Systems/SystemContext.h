//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "ResourceMapper.h"
#include "InputManager/InputManager.h"

namespace RNGOEngine
{
    namespace Utilities
    {
        class JobSystem;
    }

    namespace Events
    {
        class EventQueue;
    }

    namespace AssetHandling
    {
        class AssetManager;
    }

    namespace Core
    {
        class SceneManager;
        class InputManager;
        
        namespace Renderer
        {
            class RenderAPI;
        }
    }
}

namespace RNGOEngine::Systems
{
    struct EngineSystemContext
    {
        // Should be a time-span, but for now we use a float.
        float deltaTime = 0.0f;

        // TODO: This should probably not be owned by the SystemContext.
        Resources::ResourceMapper resourceMapper;

        // TODO: Ugly pointer bonanza over here
        RNGOEngine::Core::SceneManager* sceneManager = nullptr;
        RNGOEngine::Core::InputManager* inputManager = nullptr;
        Utilities::JobSystem* jobSystem = nullptr;
        Events::EventQueue* eventQueue = nullptr;
        AssetHandling::AssetManager* assetManager = nullptr;
        RNGOEngine::Core::Renderer::RenderAPI* renderer = nullptr;
    };

    struct SystemContext
    {
        // Should be a time-span, but for now we use a float.
        float deltaTime = 0.0f;

        // TODO: This should probably not be owned by the SystemContext.
        Resources::ResourceMapper resourceMapper;

        // TODO: Ugly pointer bonanza over here
        RNGOEngine::Core::SceneManager* sceneManager = nullptr;
        RNGOEngine::Core::InputManager* inputManager = nullptr;
        Utilities::JobSystem* jobSystem = nullptr;
        Events::EventQueue* eventQueue = nullptr;
        AssetHandling::AssetManager* assetManager = nullptr;
    };
}