//
// Created by Oskar.Norberg on 2025-11-04.
//

#pragma once
#include "Assets/AssetLoader.h"

namespace RNGOEngine::AssetHandling
{
    struct BootstrapContext
    {
        AssetLoader& Loader;

        // Model Importer Settings
        bool DoFlipUVs;
    };
    
    // TODO: I don't like this design, but it is better than having everything in the Engine source-file.
    class AssetImporterBootstrapper
    {
    public:
        static void Bootstrap(BootstrapContext& context);
    };
}