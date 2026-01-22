//
// Created by Oskar.Norberg on 2026-01-22.
//

#pragma once
#include "Assets/Asset.h"

namespace RNGOEngine::AssetHandling
{
    class ScriptAsset : public Asset
    {
    public:
        ScriptAsset(AssetHandle&& handle, std::string&& script)
            : Asset(std::move(handle)), m_script(std::move(script))
        {
        }

        std::string_view GetScript() const
        {
            return m_script;
        }

    private:
        std::string m_script;
    };

    struct ScriptMetadata : AssetMetadata
    {
    };
}