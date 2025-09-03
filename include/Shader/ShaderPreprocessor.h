//
// Created by Oskar.Norberg on 2025-09-02.
//

#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace RNGOEngine::ShaderPreProcessor
{
    constexpr const auto INCLUDE_DIRECTIVE = "#include";
    
    class ShaderPreProcessor
    {
    public:
        ShaderPreProcessor();
        std::string Parse(std::string_view source);

        void AddDefinition(std::string_view name, std::string_view value);
        void RemoveDefinition(std::string_view name);

    private:
        void ParseTokens(std::string& source) const;
        void ParseIncludes(std::string& source) const;
        
        void ParseForDefinitions(const std::string& token, std::string& source) const;

        // Map of token to a function to process that token. Function takes in a const ref to the token and a ref to string to mutate.
        std::unordered_map<std::string, std::function<void(const std::string&, std::string&)>> m_tokens;
        std::unordered_map<std::string, std::string> m_definitions;
    };
}