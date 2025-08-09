//
// Created by Oskar.Norberg on 2025-08-08.
//

#pragma once

#include <chrono>
#include <iostream>

enum StopwatchPrecision
{
    NANOSECONDS,
    MILLISECONDS,
    SECONDS
};

class STDOUTStopwatch
{
public:
    STDOUTStopwatch(std::string title, StopwatchPrecision precision, bool clearPrint = false)
        : m_precision(precision), m_title(std::move(title)), m_clearPrint(clearPrint)
    {
        m_startTime = std::chrono::high_resolution_clock::now();
    }

    STDOUTStopwatch(std::string title, StopwatchPrecision precision)
        : STDOUTStopwatch(title, precision, false)
    {
    }

    STDOUTStopwatch(StopwatchPrecision precision)
        : STDOUTStopwatch("", precision, false)
    {
    }

    STDOUTStopwatch(bool clearPrint, StopwatchPrecision precision)
        : STDOUTStopwatch("", precision, clearPrint)
    {
    }

    STDOUTStopwatch(bool clearPrint)
        : STDOUTStopwatch("", MILLISECONDS, clearPrint)
    {
    }

    STDOUTStopwatch(std::string title)
        : STDOUTStopwatch(title, MILLISECONDS, false)
    {
    }

    STDOUTStopwatch()
        : STDOUTStopwatch("", MILLISECONDS, false)
    {
    }

    ~STDOUTStopwatch()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsed = endTime - m_startTime;

        if (!m_title.empty())
        {
            std::cout << m_title << " - ";
        }

        std::string precisionString;
        double count = 0.0;

        switch (m_precision)
        {
            case NANOSECONDS:
            {
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
                count = duration.count();
                precisionString = " ns";
                break;
            }
            case MILLISECONDS:
            {
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
                count = duration.count();
                precisionString = "ms";
                break;
            }
            case SECONDS:
            {
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
                count = duration.count();
                precisionString = "s";
                break;
            }
            default:
            {
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
                count = duration.count();
                precisionString = "ns";
            }
        }

        if (m_clearPrint)
        {
            std::cout << count << std::endl;
        }
        else
        {
            std::cout << "Elapsed time: " << count << " " << precisionString << std::endl;
        }
    }

private:
    StopwatchPrecision m_precision;
    std::string m_title;
    std::chrono::high_resolution_clock::time_point m_startTime;
    bool m_clearPrint;
};