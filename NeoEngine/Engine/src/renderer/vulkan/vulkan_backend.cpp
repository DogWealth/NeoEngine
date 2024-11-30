//
// Created by NeoLiu on 2024/11/30.
//

#include "vulkan_backend.h"
namespace NeoEngine {

    bool VulkanBackend::Initialize(const char *application_name) {
        //TODO:
        context.allocator = nullptr;

        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = application_name;
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Neo Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);


        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount = 0;
        create_info.ppEnabledExtensionNames = nullptr;
        create_info.enabledLayerCount = 0;
        create_info.ppEnabledLayerNames = nullptr;

        VkResult result = vkCreateInstance(&create_info, context.allocator, &context.instance);
        if(result != VK_SUCCESS) {
            NEO_ERROR("vkCreateInstance failed with result: %u", result);
            return false;
        }

        NEO_INFO("Vulkan renderer initialized successfully.")

        return true;
    }

    void VulkanBackend::Shutdown() {

    }

    void VulkanBackend::OnResized(uint16_t width, uint16_t height) {

    }

    bool VulkanBackend::BeginFrame(float delta_time) {
        return true;
    }

    bool VulkanBackend::EndFrame(float delta_time) {
        return true;
    }
}