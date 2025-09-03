//
// Created by Oskar.Norberg on 2025-09-02.
//

#include "Shader/ShaderPreprocessor.h"

#include <cassert>
#include <stack>

#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

namespace RNGOEngine::ShaderPreProcessor
{
    ShaderPreProcessor::ShaderPreProcessor()
    {
    }

    std::string ShaderPreProcessor::Parse(const std::string_view source)
    {
        std::string processedSource(source);

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

            // Ugly string copy
            const auto includedFile = Utilities::IO::ReadFile(
                "vendor/RNGOEngine-Core/assets/shaders/includes/" + std::string(includePath));

            source.replace(it, endLineIt - it + 1, includedFile);
            source.insert(it + includedFile.size(), "\n");
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