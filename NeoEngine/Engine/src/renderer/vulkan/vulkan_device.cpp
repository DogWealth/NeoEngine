//
// Created by NeoLiu on 2024/12/1.
//

#include "vulkan_device.h"
#include "core/nstring.h"
namespace NeoEngine {
    bool VulkanDevice::Create(VulkanContext* context) {
        if(!SelectPhysicalDevice(context)) {
            return false;
        }
        return true;
    }

    void VulkanDevice::Destroy(VulkanContext* context) {

    }

    bool VulkanDevice::SelectPhysicalDevice(VulkanContext* context) {
        uint32_t physical_device_count = 0;
        VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, nullptr));
        if(physical_device_count == 0) {
            NEO_FATAL("No Devices which support Vulkan were found.");
            return false;
        }

        VkPhysicalDevice physical_devices[physical_device_count];
        VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, physical_devices));
        for(uint32_t i = 0; i < physical_device_count; ++i) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(physical_devices[i], &properties);

            VkPhysicalDeviceFeatures features;
            vkGetPhysicalDeviceFeatures(physical_devices[i], &features);

            VkPhysicalDeviceMemoryProperties memory;
            vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &memory);

            //TODO:These requirements should probably be driven by engine
            VulkanPhysicalDeviceRequirements requirements{};
            requirements.graphics = true;
            requirements.present = true;
            requirements.transfer = true;
            //NOTE: Enable this if compute will be required
            //requirements.compute = true;
            requirements.sampler_anisotropy = true;
            requirements.discrete_gpu = true;
            requirements.device_extension_names.Clear();
            requirements.device_extension_names.PushBack(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

            VulkanPhysicalDeviceQueueFamilyInfo queue_info{};
            bool result = PhysicalDeviceMeetsRequirements(physical_devices[i],
                                                          context->surface,
                                                          properties,
                                                          features,
                                                          requirements,
                                                          &queue_info,
                                                          &context->device.swapchain_support);

            if(result) {
                NEO_INFO("Selected device: '%s'.", properties.deviceName);
                switch (properties.deviceType) {
                    default:
                    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                        NEO_INFO("GPU type is Unknown.");
                        break;
                    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                        NEO_INFO("GPU type is Integrated.");
                        break;
                    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                        NEO_INFO("GPU type is Descrete.");
                        break;
                    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                        NEO_INFO("GPU type is Virtual.");
                        break;
                    case VK_PHYSICAL_DEVICE_TYPE_CPU:
                        NEO_INFO("GPU type is CPU.");
                        break;
                }

                NEO_INFO(
                        "GPU Driver version: %d.%d.%d",
                        VK_VERSION_MAJOR(properties.driverVersion),
                        VK_VERSION_MINOR(properties.driverVersion),
                        VK_VERSION_PATCH(properties.driverVersion)
                );

                NEO_INFO(
                        "Vulkan API version: %d.%d.%d",
                        VK_VERSION_MAJOR(properties.apiVersion),
                        VK_VERSION_MINOR(properties.apiVersion),
                        VK_VERSION_PATCH(properties.apiVersion)
                );

                //Memory Information
                for(uint32_t j = 0; j < memory.memoryHeapCount; ++j) {
                    float memory_size_gib = static_cast<float>(memory.memoryHeaps[j].size) / 1024.f / 1024.f / 1024.f;
                    if(memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                        NEO_INFO("Local GPU memory: %.2f GiB", memory_size_gib);
                    }else {
                        NEO_INFO("Shared system memory: %.2f GiB", memory_size_gib);
                    }
                }

                context->device.physical_device = physical_devices[i];
                context->device.graphics_queue_index = queue_info.graphics_family_index;
                context->device.present_queue_index = queue_info.present_family_index;
                context->device.transfer_queue_index = queue_info.transfer_family_index;
                //NOTE:Set compute index here if needed

                //Keep a copy of properties, feature and memory info for later use
                context->device.properties = properties;
                context->device.features = features;
                context->device.memory = memory;

                break;
            }
        }

        if(!context->device.physical_device) {
            NEO_ERROR("No physical devices were found which meet the requirements.");
            return false;
        }

        return true;
    }

    bool VulkanDevice::PhysicalDeviceMeetsRequirements(VkPhysicalDevice device, VkSurfaceKHR surface,
                                                       const VkPhysicalDeviceProperties &properties,
                                                       const VkPhysicalDeviceFeatures &features,
                                                       const VulkanPhysicalDeviceRequirements &requirements,
                                                       VulkanPhysicalDeviceQueueFamilyInfo *out_queue_family_info,
                                                       VulkanSwapchainSupportInfo *out_swapchain_support) {

        out_queue_family_info->graphics_family_index = -1;
        out_queue_family_info->present_family_index = -1;
        out_queue_family_info->compute_family_index = -1;
        out_queue_family_info->transfer_family_index = -1;

        if(requirements.discrete_gpu) {
            if(properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                NEO_INFO("Device is not a discrete GPU, and one is required. Skipping");
                return false;
            }
        }

        uint32_t  queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
        VkQueueFamilyProperties queue_families[queue_family_count];
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

        NEO_INFO("Graphics | Present | Compute | Transfer | Name");
        uint8_t  min_transfer_score = 255;
        for(uint32_t i = 0; i < queue_family_count; ++i) {
            uint8_t  current_transfer_score = 0;

            if(queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                out_queue_family_info->graphics_family_index = i;
                ++current_transfer_score;
            }

            if(queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                out_queue_family_info->compute_family_index = i;
                ++current_transfer_score;
            }

            if(queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
                if(current_transfer_score <= min_transfer_score) {
                    out_queue_family_info->transfer_family_index = i;
                }
            }

            VkBool32 supports_present = VK_FALSE;
            VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supports_present));
            if(supports_present) {
                out_queue_family_info->present_family_index = i;
            }
        }

        NEO_INFO("     %d   |     %d   |     %d   |      %d   | %s",
                 out_queue_family_info->graphics_family_index != -1,
                 out_queue_family_info->present_family_index != -1,
                 out_queue_family_info->compute_family_index != -1,
                 out_queue_family_info->transfer_family_index != -1,
                 properties.deviceName
                 );

        if(
                (!requirements.graphics || (requirements.graphics && out_queue_family_info->graphics_family_index != -1)) &&
                (!requirements.present || (requirements.present && out_queue_family_info->present_family_index != -1)) &&
                (!requirements.compute || (requirements.compute && out_queue_family_info->compute_family_index != -1)) &&
                (!requirements.transfer || (requirements.transfer && out_queue_family_info->transfer_family_index != -1))) {
            NEO_INFO("Device meets queue requirements.");
            NEO_TRACE("Graphics Family Index: %i", out_queue_family_info->graphics_family_index);
            NEO_TRACE("Present Family Index:  %i", out_queue_family_info->present_family_index);
            NEO_TRACE("Compute Family Index:  %i", out_queue_family_info->compute_family_index);
            NEO_TRACE("Transfer Family Index: %i", out_queue_family_info->transfer_family_index);
        }

        QuerySwapchainSupport(device, surface, out_swapchain_support);

        if(out_swapchain_support->format_count < 1 || out_swapchain_support->present_mode_count < 1) {
            if(out_swapchain_support->formats) {
                Memory::Deallocate(out_swapchain_support->formats, sizeof(VkSurfaceFormatKHR) * out_swapchain_support->format_count, MemoryTag::MEMORY_TAG_RENDERER );

            }

            if(out_swapchain_support->present_modes) {
                Memory::Deallocate(out_swapchain_support->present_modes, sizeof(VkPresentModeKHR) * out_swapchain_support->present_mode_count, MemoryTag::MEMORY_TAG_RENDERER );
            }

            NEO_INFO("Required swapchain support not present, skipping device.");
            return false;
        }

        //Device extensions
        if(requirements.device_extension_names.Size()) {
            uint32_t available_extension_count = 0;
            VkExtensionProperties* available_extension = nullptr;
            VK_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &available_extension_count, nullptr));
            if(available_extension_count) {
                available_extension = static_cast<VkExtensionProperties *>(Memory::Allocate(
                        sizeof(VkExtensionProperties) * available_extension_count, MemoryTag::MEMORY_TAG_RENDERER));
                VK_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &available_extension_count, available_extension));

                uint32_t required_extension_count = requirements.device_extension_names.Size();
                for(uint32_t i = 0; i < required_extension_count; ++i) {
                    bool found = false;
                    for(uint32_t j = 0; j < available_extension_count; ++j) {
                        if(string_equal(requirements.device_extension_names[i], available_extension[j].extensionName)) {
                            found = true;
                            break;
                        }
                    }

                    if(!found) {
                        NEO_INFO("Required extension not found: '%s', skipping device.", requirements.device_extension_names[i]);
                        Memory::Deallocate(available_extension, sizeof(VkExtensionProperties) * available_extension_count, MemoryTag::MEMORY_TAG_RENDERER);
                        return false;
                    }
                }
                Memory::Deallocate(available_extension, sizeof(VkExtensionProperties) * available_extension_count, MemoryTag::MEMORY_TAG_RENDERER);
            }
            //Sampler anisotropy
            if(requirements.sampler_anisotropy && !features.samplerAnisotropy) {
                NEO_INFO("Device does not support samplerAnisotropy, skipping.");
                return false;
            }

            //Device meets all requirements
            return true;
        }

        return false;
    }

    void VulkanDevice::QuerySwapchainSupport(VkPhysicalDevice physical_device, VkSurfaceKHR surface,
                                             VulkanSwapchainSupportInfo *out_support_info) {
        //Surface capabilities
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &out_support_info->capabilities));

        //Surface formats
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface,
                                                      &out_support_info->format_count,
                                                      nullptr));
        if(out_support_info->format_count != 0) {
            if(!out_support_info->formats) {
                out_support_info->formats = static_cast<VkSurfaceFormatKHR *>(Memory::Allocate(
                        sizeof(VkSurfaceFormatKHR) * out_support_info->format_count, MemoryTag::MEMORY_TAG_RENDERER));
            }
            VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface,
                                                          &out_support_info->format_count,
                                                          out_support_info->formats));
        }

        //Surface modes
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface,
                                                      &out_support_info->present_mode_count,
                                                      nullptr));
        if(out_support_info->present_mode_count != 0) {
            if(!out_support_info->present_modes) {
                out_support_info->present_modes = static_cast<VkPresentModeKHR *>(Memory::Allocate(
                        sizeof(VkPresentModeKHR) * out_support_info->present_mode_count, MemoryTag::MEMORY_TAG_RENDERER));
            }
            VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface,
                                                          &out_support_info->present_mode_count,
                                                          out_support_info->present_modes));
        }
    }
} // NeoEngine