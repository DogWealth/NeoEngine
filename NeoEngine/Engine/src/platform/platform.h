//
// Created by NeoLiu on 2024/10/27.
//

#ifndef PLATFORM_H
#define PLATFORM_H

namespace NeoEngine {

    class Platform {
    public:
        Platform() = default;
        virtual ~Platform() = default;

        virtual bool Startup(
            const std::string& application_name,
            int32_t x,
            int32_t y,
            int32_t width,
            int32_t height) = 0;

        virtual void Shutdown() = 0;
        virtual bool PumpMessage() = 0;

        static void ConsoleWrite(const std::string& message, uint8_t color);
        static void ConsoleWriteError(const std::string& message, uint8_t color);

        virtual void* Allocate(uint64_t size, bool aligned) = 0;
        virtual void Deallocate(void* ptr, bool aligned) = 0;

        static Platform* Create();

    protected:
        virtual void* Zero_Memory(void* ptr, uint64_t size) = 0;
        virtual void* Copy_Memory(void* dest, const void* src, uint64_t size) = 0;
        virtual void* SetMemory(void* dest, int32_t value, uint64_t size) = 0;

        virtual double GetAbsoluteTime() = 0;

        virtual void Sleep_(uint64_t ms) = 0;
    };
}



#endif //PLATFORM_H
