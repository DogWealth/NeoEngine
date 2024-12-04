//
// Created by NeoLiu on 2024/10/27.
//
#include "platform.h"

#include "platform_windows.h"
#include "vulkan/vulkan.h"
namespace NeoEngine {
    Platform &Platform::GetPlatform() {
#ifdef NEO_PLATFORM_WINDOWS
        static PlatformWindows platformWindows;
        return platformWindows;
#else
        NEO_FATAL("Only implement windows platform now!")
#endif
    }

}