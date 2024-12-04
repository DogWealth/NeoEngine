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
    struct VulkanSwapchainSupportInfo {
        VkSurfaceCapabilitiesKHR capabilities;

        uint32_t  format_count;
        VkSurfaceFormatKHR* formats;

        uint32_t present_mode_count;
        VkPresentModeKHR* present_modes;
    };

    struct Device {
        VkPhysicalDevice physical_device;
        VkDevice logical_device;
        VulkanSwapchainSupportInfo swapchain_support;

        uint32_t  graphics_queue_index;
        uint32_t  present_queue_index;
        uint32_t  transfer_queue_index;

        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        VkPhysicalDeviceMemoryProperties memory;

    };

    struct VulkanContext {
#ifdef NEO_DEBUG
        VkDebugUtilsMessengerEXT debug_messenger;
#endif

        VkInstance instance;
        VkAllocationCallbacks* allocator;
        VkSurfaceKHR surface;
        Device device;
    };













}
#endif //NEOENGINE_VULKAN_TYPES_H
