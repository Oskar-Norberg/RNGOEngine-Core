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
        class RuntimeAssetRegistry;
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
        float DeltaTime = 0.0f;

        // TODO: This should probably not be owned by the SystemContext.
        // TODO: Ugly pointer bonanza over here
        Resources::ResourceMapper* EngineResourceMapper = nullptr;
        Resources::ResourceMapper* GameResourceMapper = nullptr;
        RNGOEngine::Core::SceneManager* SceneManager = nullptr;
        RNGOEngine::Core::InputManager* InputManager = nullptr;
        Utilities::JobSystem* JobSystem = nullptr;
        Events::EventQueue* EventQueue = nullptr;
        AssetHandling::AssetManager* AssetManager = nullptr;
        AssetHandling::RuntimeAssetRegistry* RuntimeAssetRegistry = nullptr;
        RNGOEngine::Core::Renderer::RenderAPI* Renderer = nullptr;
    };

    struct SystemContext
    {
        // Should be a time-span, but for now we use a float.
        float DeltaTime = 0.0f;

        // TODO: Ugly pointer bonanza over here
        Resources::ResourceMapper* ResourceMapper = nullptr;
        RNGOEngine::Core::SceneManager* SceneManager = nullptr;
        RNGOEngine::Core::InputManager* InputManager = nullptr;
        Utilities::JobSystem* JobSystem = nullptr;
        Events::EventQueue* EventQueue = nullptr;
        AssetHandling::AssetManager* AssetManager = nullptr;
    };
}