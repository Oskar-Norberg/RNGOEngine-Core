//
// Created by Oskar.Norberg on 2025-11-25.
//

#include "Logging/Logger.h"

#include "spdlog/sinks/daily_file_sink.h"

namespace RNGOEngine::Core
{
    constexpr auto LOG_PATH = "Logs/RNGOEngine.log";
    constexpr auto MAX_NR_LOG_FILES = 4;

    Logger::Logger()
        : Singleton<Logger>(this),
          m_logger(
              "RNGOEngine Log",
              {
                  // Automatically log to file.
                  std::make_shared<spdlog::sinks::daily_file_sink<std::mutex>>(
                      LOG_PATH, 0, 0, false, MAX_NR_LOG_FILES
                  ),
              }
          )
    {
        m_logger.set_level(spdlog::level::debug);

        m_logger.info("Logger initialized");
    }

    Logger::~Logger()
    {
        m_logger.info("Logger Exiting");
    }

    void Logger::AttachSink(std::shared_ptr<spdlog::sinks::sink> sink)
    {
        m_logger.sinks().emplace_back(sink);
    }
}
