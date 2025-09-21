//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "ResourceMapper.h"
#include "Utilities/JobSystem/JobSystem.h"

namespace RNGOEngine
{
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
        namespace Renderer
        {
            class IRenderer;
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
        // TODO: Please move this out of here, this needs to be a pointer.
        Utilities::JobSystem jobSystem;

        // TODO: Ugly pointer bonanza over here
        Events::EventQueue* eventQueue;
        AssetHandling::AssetManager* assetManager;
        RNGOEngine::Core::Renderer::IRenderer* renderer;
    };
    
    struct SystemContext
    {
        // Should be a time-span, but for now we use a float.
        float deltaTime = 0.0f;

        // TODO: This should probably not be owned by the SystemContext.
        Resources::ResourceMapper resourceMapper;
        Utilities::JobSystem jobSystem;

        // TODO: Ugly pointer bonanza over here
        Events::EventQueue* eventQueue;
        AssetHandling::AssetManager* assetManager;
    };
}