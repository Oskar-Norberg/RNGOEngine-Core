//
// Created by Oskar.Norberg on 2025-08-08.
//

#pragma once
#include <chrono>
#include <iostream>

class STDOUTStopwatch
{
public:
    STDOUTStopwatch()
    {
        m_startTime = std::chrono::high_resolution_clock::now();
    }

    ~STDOUTStopwatch()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - m_startTime);
        std::cout << "Elapsed time: " << duration.count() << " ns" << std::endl;
    }

private:
    std::chrono::high_resolution_clock::time_point m_startTime;
};