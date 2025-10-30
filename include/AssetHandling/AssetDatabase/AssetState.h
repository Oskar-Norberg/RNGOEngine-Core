//
// Created by Oskar.Norberg on 2025-10-28.
//

#pragma once

enum class AssetState
{
    Unregistered, // Database is not aware of asset.
    Registered, // Registered to Database, but not loaded.
    LoadedToRAM, // Asset is loaded to RAM.
    Consumed, // Asset has been consumed / unloaded from RAM.
    Destroyed, // Asset has been destroyed,
};