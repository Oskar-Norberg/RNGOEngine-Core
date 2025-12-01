//
// Created by Oskar.Norberg on 2025-09-02.
//

#pragma once

#include <expected>
#include <filesystem>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "Assets/AssetFetcher/AssetFetcher.h"
#include "Data/Shaders/ShaderDefinition.h"

namespace RNGOEngine::Shaders
{
    struct ShaderParseResult
    {
        std::string VertexShader;
        std::string FragmentShader;
    };
    
    enum class ShaderPreProcessingError
    {
        None,
        FileNotFound,
        MalformedInclude,
        TokenNotFound,
    };

    class ShaderPreProcessor
    {
    public:
        ShaderPreProcessor(std::span<const Data::Shader::ShaderDefinition> definitions = {});
        std::expected<ShaderParseResult, ShaderPreProcessingError> Parse(const std::filesystem::path& source) const;

        void AddDefinition(const Data::Shader::ShaderDefinition& definition);
        void RemoveDefinition(std::string_view name);

    private:
        ShaderParseResult SplitVertAndFrag(std::string_view source) const;

    private:
        ShaderPreProcessingError ParseTokens(std::string& source) const;
        ShaderPreProcessingError ParseIncludes(std::string& source) const;

    private:
        ShaderPreProcessingError ParseForDefinitions(const std::string& token, std::string& source) const;

    private:
        // Map of token to a function to process that token.
        // Function takes in a reference to the full source, and a reference to the token string.
        // Function returns a ShaderPreProcessingError.
        std::unordered_map<std::string, std::function<ShaderPreProcessingError(const std::string&, std::string&)>> m_tokens;
        std::unordered_map<std::string, std::string> m_definitions;

        std::vector<std::filesystem::path> m_includeDirectories;
    };
}