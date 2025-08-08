//
// Created by Oskar.Norberg on 2025-08-08.
//

#pragma once
#include <chrono>
#include <iostream>

class STDOUTStopwatch
{
public:
    STDOUTStopwatch(std::string title) : m_title(std::move(title))
    {
        m_startTime = std::chrono::high_resolution_clock::now();
    }

    STDOUTStopwatch() : STDOUTStopwatch("")
    {
    }

    ~STDOUTStopwatch()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - m_startTime);

        if (!m_title.empty())
        {
            std::cout << m_title << " - ";
        }

        std::cout << "Elapsed time: " << duration.count() << " ns" << std::endl;
    }

private:
    std::string m_title;
    std::chrono::high_resolution_clock::time_point m_startTime;
};