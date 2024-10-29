//
// Created by NeoLiu on 2024/10/27.
//
#include "platform.h"

#include "platform_windows.h"
namespace NeoEngine {
    void Platform::ConsoleWrite(const std::string& message, uint8_t color) {
#ifdef NEO_PLATFORM_WINDOWS
        PlatformWindows::ConsoleWrite(message, color);
#else
        NEO_FATAL("Only implement windows platform now!")
#endif
    }

    void Platform::ConsoleWriteError(const std::string& message, uint8_t color) {
#ifdef NEO_PLATFORM_WINDOWS
        PlatformWindows::ConsoleWriteError(message, color);
#else
        NEO_FATAL("Only implement windows platform now!")
#endif
    }

    Platform *Platform::Create() {
#ifdef NEO_PLATFORM_WINDOWS
        return new  PlatformWindows();
#else
        NEO_FATAL("Only implement windows platform now!")
#endif
    }

}