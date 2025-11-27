//
// Created by Oskar.Norberg on 2025-11-27.
//

#pragma once

#include <filesystem>
#include <span>
#include <string_view>

// Including Asset here is a bit weird. But EngineConfig needs to know about AssetTypes.
#include "Assets/Asset.h"

namespace RNGOEngine::Data
{
    enum class RenderType
    {
        Headless,
        GLFW_OpenGL,
    };

    enum class Pipeline
    {
        Forward,
        ForwardPlus,
        Deferred,
    };

    enum class ThreadingPolicy 
    {
        SingleThreaded,
        MultiThreaded,
    };

    struct EngineConfig
    {
        RenderType RenderType = RenderType::Headless;
        Pipeline Pipeline = Pipeline::Forward;
        size_t Width = 800;
        size_t Height = 600;
        std::string_view Name = "RNGOEngine Application";
        ThreadingPolicy threadingPolicy = ThreadingPolicy::MultiThreaded;

        std::span<const std::pair<std::filesystem::path, AssetHandling::AssetType>> AssetPaths;
        bool LogToVectorSink = false;
    };
}