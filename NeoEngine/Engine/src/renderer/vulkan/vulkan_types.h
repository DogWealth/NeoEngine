//
// Created by NeoLiu on 2024/11/30.
//

#ifndef NEOENGINE_VULKAN_TYPES_H
#define NEOENGINE_VULKAN_TYPES_H
#include "defines.h"
#include <vulkan/vulkan.h>
namespace NeoEngine {
#define VK_CHECK(expr)              \
{                                   \
    NEO_ASSERT(expr == VK_SUCCESS); \
}

    struct VulkanContext {
        VkInstance instance;
        VkAllocationCallbacks* allocator;

#ifdef NEO_DEBUG
        VkDebugUtilsMessengerEXT debug_messenger;
#endif
    };













}
#endif //NEOENGINE_VULKAN_TYPES_H
