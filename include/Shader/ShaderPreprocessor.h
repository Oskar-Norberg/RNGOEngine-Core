//
// Created by Oskar.Norberg on 2025-09-02.
//

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace RNGOEngine::ShaderPreProcessor
{
    class ShaderPreProcessor
    {
    public:
        std::string Parse(std::string_view source);
        
        void AddDefinition(std::string_view name, std::string_view value);
        void RemoveDefinition(std::string_view name);

    private:
        // This is copying a string, slightly ugly.
        std::string ParseForDefinitions(std::string source) const;
        
        std::unordered_map<std::string, std::string> m_definitions;
    };
}