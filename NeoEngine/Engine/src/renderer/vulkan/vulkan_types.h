//
// Created by NeoLiu on 2024/11/30.
//

#ifndef NEOENGINE_VULKAN_TYPES_H
#define NEOENGINE_VULKAN_TYPES_H
#include "defines.h"
#include <vulkan/vulkan.h>
namespace NeoEngine {

    struct VulkanContext {
        VkInstance instance;
        VkAllocationCallbacks* allocator;
    };
}
#endif //NEOENGINE_VULKAN_TYPES_H
