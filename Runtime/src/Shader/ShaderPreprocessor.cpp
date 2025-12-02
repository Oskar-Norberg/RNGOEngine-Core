//
// Created by Oskar.Norberg on 2025-09-02.
//

#include "Shader/ShaderPreprocessor.h"

#include <string.h>

#include <cstring>
#include <unordered_set>

#include "Data/Shaders/ShaderSpecification.h"
#include "Renderer/DrawQueue.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"
#include "Utilities/RNGOAsserts.h"

#ifndef ENGINE_SHADERS_DIR
#error "ENGINE_SHADERS_DIR is not defined."
#endif

namespace RNGOEngine::Shaders
{
    ShaderPreProcessor::ShaderPreProcessor(std::span<const Data::Shader::ShaderDefinition> definitions)
    {
        for (const auto& definition : definitions)
        {
            AddDefinition(definition);
        }
    }

    std::expected<ShaderParseResult, ShaderPreProcessingError> ShaderPreProcessor::Parse(
        const std::filesystem::path& source
    ) const
    {
        // TODO: Should this be getting the path from asset fetcher?
        // This should be passed an actualized path and the includes should be resolved through the Fetcher.
        const auto foundPath =
            AssetHandling::AssetFetcher::GetInstance().GetPath(AssetHandling::AssetType::Shader, source);

        if (!foundPath.has_value())
        {
            RNGO_ASSERT(false && "Shader not found.");
            return std::unexpected(ShaderPreProcessingError::FileNotFound);
        }

        std::string combinedShader = Utilities::IO::ReadFile(foundPath.value());
        auto splitResult = SplitVertAndFrag(combinedShader);

        if (!splitResult)
        {
            return std::unexpected(splitResult.error());
        }

        auto& splitShaders = splitResult.value();

        auto processLambda = [this](std::string& shaderSource) -> ShaderPreProcessingError
        {
            const auto includeError = ParseIncludes(shaderSource);
            if (includeError != ShaderPreProcessingError::None)
            {
                return includeError;
            }

            const auto tokenError = ParseTokens(shaderSource);
            if (tokenError != ShaderPreProcessingError::None)
            {
                return tokenError;
            }

            return ShaderPreProcessingError::None;
        };

        ShaderPreProcessingError vertexLambdaResult = processLambda(splitShaders.VertexShader);
        ShaderPreProcessingError fragmentLambdaResult = processLambda(splitShaders.FragmentShader);

        if (vertexLambdaResult != ShaderPreProcessingError::None)
        {
            return std::unexpected(vertexLambdaResult);
        }

        if (fragmentLambdaResult != ShaderPreProcessingError::None)
        {
            return std::unexpected(fragmentLambdaResult);
        }

        return splitShaders;
    }

    void ShaderPreProcessor::AddDefinition(const Data::Shader::ShaderDefinition& definition)
    {
        m_definitions[std::string(definition.Name)] = std::to_string(definition.Value);
        m_tokens[std::string(definition.Name)] = [this](const std::string& token, std::string& source)
        {
            return ParseForDefinitions(token, source);
        };
    }

    void ShaderPreProcessor::RemoveDefinition(const std::string_view name)
    {
        if (const auto it = m_definitions.find(std::string(name)); it != m_definitions.end())
        {
            m_definitions.erase(it);
        }
    }

    std::expected<ShaderParseResult, ShaderPreProcessingError> ShaderPreProcessor::SplitVertAndFrag(
        const std::string_view source
    ) const
    {
        const size_t vertStart =
            source.find(Data::Shader::VERTEX_SHADER_START) + std::strlen(Data::Shader::VERTEX_SHADER_START);
        const size_t fragStart = source.find(Data::Shader::FRAGMENT_SHADER_START);

        if (vertStart == std::string::npos)
        {
            return std::unexpected(ShaderPreProcessingError::MissingVertexStart);
        }
        if (fragStart == std::string::npos)
        {
            return std::unexpected(ShaderPreProcessingError::MissingFragmentStart);
        }

        if (fragStart < vertStart)
        {
            return std::unexpected(ShaderPreProcessingError::MisorderedShaders);
        }

        const std::string sourceStr(source);

        ShaderParseResult result;
        result.VertexShader =
            sourceStr.substr(vertStart, fragStart - std::strlen(Data::Shader::FRAGMENT_SHADER_START) - 1);
        result.FragmentShader =
            sourceStr.substr(fragStart + std::strlen(Data::Shader::FRAGMENT_SHADER_START));

        return result;
    }

    ShaderPreProcessingError ShaderPreProcessor::ParseTokens(std::string& source) const
    {
        for (const auto& token : m_tokens)
        {
            const auto it = source.find(token.first);
            if (it != std::string::npos)
            {
                const auto errorCode = token.second(token.first, source);

                if (errorCode != ShaderPreProcessingError::None)
                {
                    return errorCode;
                }
            }
        }

        return ShaderPreProcessingError::None;
    }

    ShaderPreProcessingError ShaderPreProcessor::ParseIncludes(std::string& source) const
    {
        std::unordered_set<std::string> includedFiles;

        while (true)
        {
            const auto it = source.find(Data::Shader::INCLUDE_DIRECTIVE);

            if (it == std::string::npos)
            {
                break;
            }

            const auto endLineIt = source.find('\n', it);
            const auto includeBeginIt = source.find(Data::Shader::INCLUDE_START, it);
            const auto includeEndIt = source.find(Data::Shader::INCLUDE_END, includeBeginIt);

            if (includeBeginIt > endLineIt || includeEndIt > endLineIt)
            {
                RNGO_ASSERT(false && "Malformed include directive.");
                return ShaderPreProcessingError::MalformedInclude;
            }

            std::string_view includePath =
                std::string_view(source.data() + includeBeginIt + 1, includeEndIt - includeBeginIt - 1);

            if (includedFiles.contains(std::string(includePath)))
            {
                // Delete the include directive.
                source.replace(it, endLineIt - it + 1, "");
            }
            else
            {
                includedFiles.insert(std::string(includePath));

                const auto includeFilePath = AssetHandling::AssetFetcher::GetInstance().GetPath(
                    AssetHandling::AssetType::Shader, includePath
                );

                if (!includeFilePath.has_value())
                {
                    RNGO_ASSERT(false && "Failed to open include file.");
                    return ShaderPreProcessingError::FileNotFound;
                }

                if (includeFilePath.has_value())
                {
                    // TODO: Pass a file reader?
                    const auto includeFile = Utilities::IO::ReadFile(includeFilePath.value());

                    source.replace(it, endLineIt - it + 1, includeFile);
                    source.insert(it + includeFile.size(), "\n");
                }
            }
        }

        return ShaderPreProcessingError::None;
    }

    ShaderPreProcessingError ShaderPreProcessor::ParseForDefinitions(
        const std::string& token, std::string& source
    ) const
    {
        const auto tokenIt = m_definitions.find(token);

        if (tokenIt == m_definitions.end())
        {
            RNGO_ASSERT(false && "Token does not exist in definitions.");
            return ShaderPreProcessingError::TokenNotFound;
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

        return ShaderPreProcessingError::None;
    }
}