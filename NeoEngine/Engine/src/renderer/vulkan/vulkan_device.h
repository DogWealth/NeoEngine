//
// Created by NeoLiu on 2024/12/1.
//

#ifndef NEOENGINE_VULKAN_DEVICE_H
#define NEOENGINE_VULKAN_DEVICE_H
#include "vulkan_types.h"
namespace NeoEngine {
    struct VulkanPhysicalDeviceRequirements {
        bool graphics;
        bool present;
        bool compute;
        bool transfer;

        DArray<const char*> device_extension_names;
        bool sampler_anisotropy;
        bool discrete_gpu;
    };

    struct  VulkanPhysicalDeviceQueueFamilyInfo {
        uint32_t graphics_family_index;
        uint32_t present_family_index;
        uint32_t compute_family_index;
        uint32_t transfer_family_index;
    };

    class VulkanDevice {
    public:
        static bool Create(VulkanContext* context);
        static void Destroy(VulkanContext* context);
        static void QuerySwapchainSupport(
                VkPhysicalDevice physical_device,
                VkSurfaceKHR surface,
                VulkanSwapchainSupportInfo* out_support_info
                );

    private:
        static bool SelectPhysicalDevice(VulkanContext* context);
        static bool PhysicalDeviceMeetsRequirements(
                VkPhysicalDevice device,
                VkSurfaceKHR surface,
                const VkPhysicalDeviceProperties& properties,
                const VkPhysicalDeviceFeatures& features,
                const VulkanPhysicalDeviceRequirements& requirements,
                VulkanPhysicalDeviceQueueFamilyInfo* out_queue_family_info,
                VulkanSwapchainSupportInfo* out_swapchain_support
                );
    };

} // NeoEngine

#endif //NEOENGINE_VULKAN_DEVICE_H
