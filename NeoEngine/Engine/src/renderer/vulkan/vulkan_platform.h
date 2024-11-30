//
// Created by NeoLiu on 2024/11/30.
//

#ifndef NEOENGINE_VULKAN_PLATFORM_H
#define NEOENGINE_VULKAN_PLATFORM_H
#include "containers/darray.h"
namespace NeoEngine {
    //Get the required extension name for specific platform
    void PlatformGetRequiredExtensionNames(DArray<const char*>& dArray);
}
#endif //NEOENGINE_VULKAN_PLATFORM_H
