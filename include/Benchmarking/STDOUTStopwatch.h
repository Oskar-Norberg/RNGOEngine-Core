//
// Created by Oskar.Norberg on 2025-08-08.
//

#pragma once

#include <chrono>
#include <string>

namespace RNGOEngine::Benchmarking
{
    enum StopwatchPrecision
    {
        NANOSECONDS,
        MILLISECONDS,
        SECONDS
    };

    class STDOUTStopwatch
    {
    public:
        STDOUTStopwatch(StopwatchPrecision precision, std::string title)
            : m_precision(precision), m_title(std::move(title))
        {
            m_startTime = std::chrono::high_resolution_clock::now();
        }

        STDOUTStopwatch(StopwatchPrecision precision)
            : STDOUTStopwatch(precision, "")
        {
        }

        STDOUTStopwatch(std::string title)
            : STDOUTStopwatch(MILLISECONDS, title)
        {
        }

        STDOUTStopwatch()
            : STDOUTStopwatch(MILLISECONDS, "")
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

            std::cout << "Elapsed time: " << count << " " << precisionString << std::endl;
        }

    private:
        StopwatchPrecision m_precision;
        std::string m_title;
        std::chrono::high_resolution_clock::time_point m_startTime;
    };
}