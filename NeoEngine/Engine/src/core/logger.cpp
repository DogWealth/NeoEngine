//
// Created by NeoLiu on 2024/10/26.
//
#include "logger.h"
#include "platform/platform.h"

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

        constexpr uint32_t msg_len = 1024;
        char buffer[msg_len] = {}; // zero initialization

        __builtin_va_list args;
        va_start(args, message);
        vsnprintf(buffer, msg_len, message, args);
        va_end(args);

        char out_buffer[msg_len] = {};
        const auto level_ = static_cast<uint8_t>(level);
        sprintf(out_buffer, "%s%s\n", level_str[level_], buffer);

        if(is_error) {
            Platform::PlatformConsoleWriteError(out_buffer, level_);
        } else {
            Platform::PlatformConsoleWrite(out_buffer, level_);
        }
    }
}
