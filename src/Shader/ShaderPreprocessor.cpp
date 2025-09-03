//
// Created by Oskar.Norberg on 2025-09-02.
//

#include "Shader/ShaderPreprocessor.h"

#include <cassert>
#include <stack>

namespace RNGOEngine::ShaderPreProcessor
{
    std::string ShaderPreProcessor::Parse(std::string_view source)
    {
        std::string processedSource(source);

        for (const auto& token : m_tokens)
        {
            const auto it = source.find(token.first);
            if (it != std::string::npos)
            {
                token.second(token.first, processedSource);
            }
        }

        return processedSource;
    }

    void ShaderPreProcessor::AddDefinition(std::string_view name, std::string_view value)
    {
        m_definitions[std::string(name)] = std::string(value);
        m_tokens[std::string(name)] = [this](const std::string& token, std::string& source)
        {
            ParseForDefinitions(token, source);
        };
    }

    void ShaderPreProcessor::RemoveDefinition(std::string_view name)
    {
        const auto it = m_definitions.find(std::string(name));

        if (it != m_definitions.end())
        {
            m_definitions.erase(it);
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