//
// Created by NeoLiu on 2024/10/27.
//

#ifndef PLATFORM_H
#define PLATFORM_H

namespace NeoEngine {

    class NEO_API Platform {
    public:
        Platform() = default;
        virtual ~Platform() = default;

        virtual bool PlatformStartup(
            const std::string& application_name,
            int32_t x,
            int32_t y,
            int32_t width,
            int32_t height) = 0;

        virtual void PlatformShutdown() = 0;
        virtual bool PlatformPumpMessage() = 0;

        static void PlatformConsoleWrite(const std::string& message, uint8_t color);
        static void PlatformConsoleWriteError(const std::string& message, uint8_t color);

        static Platform* Create();

    protected:
        virtual void* PlatformAllocate(uint64_t size, bool aligned) = 0;
        virtual void PlatformDeallocate(void* ptr, bool aligned) = 0;
        virtual void* PlatformZeroMemory(void* ptr, uint64_t size) = 0;
        virtual void* PlatformCopyMemory(void* dest, const void* src, uint64_t size) = 0;
        virtual void* PlatformSetMemory(void* dest, int32_t value, uint64_t size) = 0;

        virtual double PlatformGetAbsoluteTime() = 0;

        virtual void PlatformSleep(uint64_t ms) = 0;
    };
}



#endif //PLATFORM_H
