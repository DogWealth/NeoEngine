//
// Created by NeoLiu on 2024/10/26.
//
#include "logger.h"

namespace NeoEngine {
    bool Logger::InitializeLogger() {
        //TODO
        return true;
    }

    void Logger::ShutdownLogger() {
        //TODO
    }

    void Logger::Log(LogLevel level, const char* message, ...) {
        const char* level_str[6] = {"[FATAL]: ", "[ERROR]: ", "[WARNING]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
        bool is_error = level < LogLevel::LOG_LEVEL_WARNING;

        char buffer[1024] = {}; // zero initialization

        __builtin_va_list args;
        va_start(args, message);
        vsnprintf(buffer, 1024, message, args);
        va_end(args);

        char out_buffer[1024] = {};
        sprintf(out_buffer, "%s%s\n", level_str[static_cast<uint8_t>(level)], buffer);
        printf("%s", out_buffer);
    }
}
