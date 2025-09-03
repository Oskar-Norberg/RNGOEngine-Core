//
// Created by Oskar.Norberg on 2025-09-02.
//

#include "Shader/ShaderPreprocessor.h"

#include <cassert>

#include "Renderer/DrawQueue.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

#ifndef ENGINE_SHADERS_DIR
    #error "ENGINE_SHADERS_DIR is not defined."
#endif

namespace RNGOEngine::Shaders
{
    ShaderPreProcessor::ShaderPreProcessor(const AssetHandling::AssetFileFetcher& assetFetcher)
        : m_assetFetcher(assetFetcher)
    {
        AddDefinition("NR_OF_POINTLIGHTS", std::to_string(Core::Renderer::NR_OF_POINTLIGHTS));
        AddDefinition("NR_OF_SPOTLIGHTS", std::to_string(Core::Renderer::NR_OF_SPOTLIGHTS));
    }

    std::string ShaderPreProcessor::Parse(const std::filesystem::path& source) const
    {
        const auto foundPath = m_assetFetcher.GetShaderPath(source);

        if (!foundPath.has_value())
        {
            assert(false && "Shader not found.");
            return {};
        }
        
        std::string processedSource = Utilities::IO::ReadFile(foundPath.value());

        ParseTokens(processedSource);
        ParseIncludes(processedSource);

        return processedSource;
    }

    void ShaderPreProcessor::AddDefinition(const std::string_view name, const std::string_view value)
    {
        m_definitions[std::string(name)] = std::string(value);
        m_tokens[std::string(name)] = [this](const std::string& token, std::string& source)
        {
            ParseForDefinitions(token, source);
        };
    }

    void ShaderPreProcessor::RemoveDefinition(const std::string_view name)
    {
        if (const auto it = m_definitions.find(std::string(name)); it != m_definitions.end())
        {
            m_definitions.erase(it);
        }
    }

    void ShaderPreProcessor::ParseTokens(std::string& source) const
    {
        for (const auto& token : m_tokens)
        {
            const auto it = source.find(token.first);
            if (it != std::string::npos)
            {
                token.second(token.first, source);
            }
        }
    }

    void ShaderPreProcessor::ParseIncludes(std::string& source) const
    {
        while (true)
        {
            const auto it = source.find(INCLUDE_DIRECTIVE);

            if (it == std::string::npos)
            {
                break;
            }

            // Only supports diamond bracket includes for now.
            const auto endLineIt = source.find('\n', it);
            const auto includeBeginIt = source.find('<', it);
            const auto includeEndIt = source.find('>', includeBeginIt);

            if (includeBeginIt > endLineIt || includeEndIt > endLineIt)
            {
                assert(false && "Malformed include directive.");
                break;
            }

            std::string_view includePath = std::string_view(
                source.data() + includeBeginIt + 1,
                includeEndIt - includeBeginIt - 1
            );

            const auto includeFilePath = m_assetFetcher.GetShaderPath(includePath);

            assert(includeFilePath.has_value() && "Failed to open include file.");

            if (includeFilePath.has_value())
            {
                // TODO: Pass a file reader?
                const auto includeFile = Utilities::IO::ReadFile(includeFilePath.value());
                
                source.replace(it, endLineIt - it + 1, includeFile);
                source.insert(it + includeFile.size(), "\n");
            }
        }
    }

    void ShaderPreProcessor::ParseForDefinitions(const std::string& token, std::string& source) const
    {
        const auto tokenIt = m_definitions.find(token);

        if (tokenIt == m_definitions.end())
        {
            assert(false && "Token does not exist in definitions.");
            return;
        }

        while (true)
        {
            if (const auto it = source.find(token); it != std::string::npos)
            {
                source.replace(it, token.length(), tokenIt->second);
            }
            else
            {
                break;
            }
        }
    }
}