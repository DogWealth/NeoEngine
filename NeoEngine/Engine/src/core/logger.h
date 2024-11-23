//
// Created by NeoLiu on 2024/10/26.
//

#ifndef LOGGER_H
#define LOGGER_H

namespace NeoEngine {
    enum class LogLevel : uint8_t {
        LOG_LEVEL_FATAL = 0,
        LOG_LEVEL_ERROR = 1,
        LOG_LEVEL_WARNING = 2,
        LOG_LEVEL_INFO = 3,
        LOG_LEVEL_DEBUG = 4,
        LOG_LEVEL_TRACE = 5
    };

    class NEO_API Logger {
    public:
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger(Logger&&) = delete;
        Logger& operator=(Logger&&) = delete;
        static Logger& GetLogger() {
            static Logger logger;
            return logger;
        }

        bool InitializeLogger();
        void ShutdownLogger();
        void Log(LogLevel level, const char* message, ...);

    private:
        Logger() = default;
    };


}

#define LOG_WARNING_ENABLED 1
#define LOG_TRACE_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_INFO_ENABLED 1

#if NEO_RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

#define NEO_FATAL(message, ...) NeoEngine::Logger::GetLogger().Log(NeoEngine::LogLevel::LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef NEO_ERROR
#define NEO_ERROR(message, ...) NeoEngine::Logger::GetLogger().Log(NeoEngine::LogLevel::LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARNING_ENABLED == 1
#define NEO_WARNING(message, ...) NeoEngine::Logger::GetLogger().Log(NeoEngine::LogLevel::LOG_LEVEL_WARNING, message, ##__VA_ARGS__);
#else
#define NEO_WARNING(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define NEO_INFO(message, ...) NeoEngine::Logger::GetLogger().Log(NeoEngine::LogLevel::LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define NEO_INFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define NEO_DEBUG_MSG(message, ...) NeoEngine::Logger::GetLogger().Log(NeoEngine::LogLevel::LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define NEO_DEBUG_MSG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define NEO_TRACE(message, ...) NeoEngine::Logger::GetLogger().Log(NeoEngine::LogLevel::LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define NEO_TRACE(message, ...)
#endif

#endif //LOGGER_H
