//
// Created by Oskar.Norberg on 2025-09-02.
//

#include "Shader/ShaderPreprocessor.h"

#include <stack>

namespace RNGOEngine::ShaderPreProcessor
{
    std::string ShaderPreProcessor::Parse(std::string_view source)
    {
        std::string processedSource = ParseForDefinitions(std::string(source));

        return processedSource;
    }

    void ShaderPreProcessor::AddDefinition(std::string_view name, std::string_view value)
    {
        m_definitions[std::string(name)] = std::string(value);
    }

    void ShaderPreProcessor::RemoveDefinition(std::string_view name)
    {
        const auto it = m_definitions.find(std::string(name));

        if (it != m_definitions.end())
        {
            m_definitions.erase(it);
        }
    }

    std::string ShaderPreProcessor::ParseForDefinitions(const std::string source) const
    {
        std::string processedSource = source;

        for (const auto& [key, value] : m_definitions)
        {
            while (true)
            {
                if (const auto it = processedSource.find(key); it != std::string::npos)
                {
                    processedSource.replace(it, key.length(), value);
                }
                else
                {
                    break;
                }
            }
        }

        return processedSource;
    }
}