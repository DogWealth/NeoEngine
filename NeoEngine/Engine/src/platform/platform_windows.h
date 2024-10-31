//
// Created by NeoLiu on 2024/10/27.
//

#ifndef PLATFORM_WINDOWS_H
#define PLATFORM_WINDOWS_H

#include "platform.h"

#include <windows.h>
#include <windowsx.h>

namespace NeoEngine {
    struct PlatformState {
        HINSTANCE hInstance;
        HWND hWnd;
    };

    class PlatformWindows : public Platform {
    public:
        PlatformWindows();
        ~PlatformWindows() override;

        bool Startup(
            const std::string& application_name,
            int32_t x,
            int32_t y,
            int32_t width,
            int32_t height) override;

        void Shutdown() override;

        void ConsoleWrite(const std::string& message, uint8_t color) override;
        void ConsoleWriteError(const std::string& message, uint8_t color) override;

        void* Allocate(uint64_t size, bool aligned) override;
        void Deallocate(void* ptr, bool aligned) override;

        void* Zero_Memory(void* ptr, uint64_t size) override;
        void* Copy_Memory(void* dest, const void* src, uint64_t size) override;
        void* SetMemory(void* dest, int32_t value, uint64_t size) override;

    private:

        bool PumpMessage() override;
        double GetAbsoluteTime() override;
        void Sleep_(uint64_t ms) override;

    private:
        PlatformState* platform_state_;
        double clock_frequency_;
        LARGE_INTEGER start_time_;
    };
}



#endif //PLATFORM_WINDOWS_H
