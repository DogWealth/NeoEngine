//
// Created by NeoLiu on 2024/11/30.
//

#include "vulkan_backend.h"
#include "core/nstring.h"
#include "containers/darray.h"
#include "platform/platform.h"
#include "vulkan_platform.h"

namespace NeoEngine {

    bool VulkanBackend::Initialize(const char *application_name) {
        //TODO:
        context_.allocator = nullptr;

        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = application_name;
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Neo Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);


        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;

        //obtain a list of required extension
        DArray<const char*> required_extensions;
        required_extensions.PushBack(VK_KHR_SURFACE_EXTENSION_NAME);
        PlatformGetRequiredExtensionNames(required_extensions);
#ifdef NEO_DEBUG
        required_extensions.PushBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        NEO_DEBUG_MSG("Required extensions:");
        uint32_t len = required_extensions.Size();
        for(uint32_t i = 0; i < len; ++i) {
            NEO_DEBUG_MSG(required_extensions[i]);
        }
#endif

        create_info.enabledExtensionCount = required_extensions.Size();
        create_info.ppEnabledExtensionNames = required_extensions.Data();

        //validation layers.Should only be enabled on non-release builds
        DArray<const char*> required_validation_layer;
        uint32_t required_validation_layer_count = 0;
#ifdef NEO_DEBUG
        required_validation_layer.PushBack("VK_LAYER_KHRONOS_validation");
        required_validation_layer_count = required_validation_layer.Size();

        uint32_t  available_layer_count = 0;
        VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr));
        DArray <VkLayerProperties> available_layers(available_layer_count);
        VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers.Data()));

        //verify all required layers are available
        for(uint32_t i = 0; i < required_validation_layer_count; ++i) {
            NEO_INFO("Searching for layer: %s...", required_validation_layer[i]);
            bool found = false;
            for(uint32_t j = 0; j < available_layer_count; ++j) {
                if(string_equal(required_validation_layer[i], available_layers[j].layerName)) {
                    found = true;
                    NEO_INFO("Found.");
                    break;
                }
            }

            if(!found) {
                NEO_FATAL("Required validation layer is missing: %s", required_validation_layer[i]);
                return false;
            }
        }
#endif

        create_info.enabledLayerCount = 0;
        create_info.ppEnabledLayerNames = nullptr;

        VK_CHECK(vkCreateInstance(&create_info, context_.allocator, &context_.instance));
        NEO_INFO("Vulkan instance created.")

#ifdef NEO_DEBUG
//debugger
        NEO_DEBUG_MSG("Creating vulkan debugger...")
        uint32_t  log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

        VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
        debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_create_info.messageSeverity = log_severity;
        debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        debug_create_info.pfnUserCallback = vk_debug_callback;
        debug_create_info.pUserData = nullptr;

        auto func =
                (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(context_.instance, "vkCreateDebugUtilsMessengerEXT");

        NEO_ASSERT_MSG(func, "Failed to create debug messenger!");
        VK_CHECK(func(context_.instance, &debug_create_info, context_.allocator, &context_.debug_messenger));
        NEO_DEBUG_MSG("Vulkan debugger created.");

#endif
        NEO_INFO("Vulkan renderer initialized successfully.")

        return true;
    }

    void VulkanBackend::Shutdown() {
        NEO_DEBUG_MSG("Destroying Vulkan debugger");
        if(context_.debug_messenger) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(context_.instance, "vkDestroyDebugUtilsMessengerEXT");

            func(context_.instance, context_.debug_messenger, context_.allocator);
        }

        NEO_DEBUG_MSG("Destroying Vulkan instance...");
        vkDestroyInstance(context_.instance, context_.allocator);
    }

    void VulkanBackend::OnResized(uint16_t width, uint16_t height) {

    }

    bool VulkanBackend::BeginFrame(float delta_time) {
        return true;
    }

    bool VulkanBackend::EndFrame(float delta_time) {
        return true;
    }

    VKAPI_ATTR VkBool32  VKAPI_CALL vk_debug_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
            VkDebugUtilsMessageTypeFlagsEXT message_types,
            const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
            void* user_data) {
        switch (message_severity) {
            default:
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                NEO_ERROR(callback_data->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                NEO_WARNING(callback_data->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                NEO_INFO(callback_data->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                NEO_TRACE(callback_data->pMessage);
                break;

        }

        return VK_FALSE;
    }
}