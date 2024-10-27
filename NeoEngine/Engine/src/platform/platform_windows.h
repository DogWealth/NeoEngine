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

        bool PlatformStartup(
            const std::string& application_name,
            int32_t x,
            int32_t y,
            int32_t width,
            int32_t height) override;

        void PlatformShutdown() override;

        static void PlatformConsoleWrite(const std::string& message, uint8_t color);
        static void PlatformConsoleWriteError(const std::string& message, uint8_t color);

    private:

        bool PlatformPumpMessage() override;
        void* PlatformAllocate(uint64_t size, bool aligned) override;
        void PlatformDeallocate(void* ptr, bool aligned) override;
        void* PlatformZeroMemory(void* ptr, uint64_t size) override;
        void* PlatformCopyMemory(void* dest, const void* src, uint64_t size) override;
        void* PlatformSetMemory(void* dest, int32_t value, uint64_t size) override;
        double PlatformGetAbsoluteTime() override;
        void PlatformSleep(uint64_t ms) override;

    private:
        PlatformState* platform_state_;
        double clock_frequency_;
        LARGE_INTEGER start_time_;
    };
}



#endif //PLATFORM_WINDOWS_H
