//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace RNGOEngine::Utilities::IO
{
    static bool FileExists(const std::filesystem::path& filePath)
    {
        const std::ifstream file(filePath);
        return file.good();
    }
    
    ///
    /// Simply reads a file and returns its content as a string.
    /// Probably not the quickest thing in the world definitely not the safest thing in the world.
    /// But god-damn it works.
    /// 
    static std::string ReadFile(const std::filesystem::path& filePath)
    {
        std::string content;
    
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
        try
        {
            file.open(filePath);
            std::stringstream fileStream;

            fileStream << file.rdbuf();

            file.close();

            content = fileStream.str();
        }
        catch (std::ifstream::failure&)
        {
        }

        return content;
    }
}