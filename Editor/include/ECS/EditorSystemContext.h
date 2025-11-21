//
// Created by Oskar.Norberg on 2025-11-17.
//

#pragma once

namespace RNGOEngine
{
    namespace Systems
    {
        namespace Resources
        {
            class ResourceMapper;
        }
    }
    namespace AssetHandling
    {
        class AssetManager;
    }
    namespace Events
    {
        class EventQueue;
    }
    namespace Utilities
    {
        class JobSystem;

    }
    namespace Core
    {
        namespace Renderer
        {
            class RenderAPI;
        }
        class InputManager;
        class SceneManager;

    }
}

namespace RNGOEngine::Editor
{
    struct EditorSystemContext
    {
        // Should be a time-span, but for now we use a float.
        float deltaTime = 0.0f;

        

        // TODO: This should probably not be owned by the SystemContext.
        // TODO: Ugly pointer bonanza over here
        Systems::Resources::ResourceMapper* engineResourceMapper = nullptr;
        Systems::Resources::ResourceMapper* gameResourceMapper = nullptr;
        RNGOEngine::Core::SceneManager* sceneManager = nullptr;
        RNGOEngine::Core::InputManager* inputManager = nullptr;
        Utilities::JobSystem* jobSystem = nullptr;
        Events::EventQueue* eventQueue = nullptr;
        AssetHandling::AssetManager* assetManager = nullptr;
        RNGOEngine::Core::Renderer::RenderAPI* renderer = nullptr;
    };
}