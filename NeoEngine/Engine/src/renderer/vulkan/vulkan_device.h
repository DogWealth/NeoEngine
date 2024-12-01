//
// Created by NeoLiu on 2024/12/1.
//

#ifndef NEOENGINE_VULKAN_DEVICE_H
#define NEOENGINE_VULKAN_DEVICE_H
#include "vulkan_types.h"
namespace NeoEngine {

    class VulkanDevice {
    public:
        static bool Create(const VulkanContext& context);
        static void Destroy(const VulkanContext& context);
    };

} // NeoEngine

#endif //NEOENGINE_VULKAN_DEVICE_H
