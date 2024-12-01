//
// Created by NeoLiu on 2024/11/30.
//
#include "renderer_backend.h"
#include "vulkan/vulkan_backend.h"
namespace NeoEngine {

    RendererBackend *RendererBackend::Create(RendererBackendType type) {
        if(type == RendererBackendType::Renderer_BACKEND_DIRECTX) {
            //TODO:
            auto* backend = static_cast<VulkanBackend*>(Memory::Allocate(sizeof(RendererBackend), MemoryTag::MEMORY_TAG_RENDERER));
            new(backend) VulkanBackend();
            NEO_DEBUG_MSG("Create Vulkan renderer");
            return backend;
        }

        NEO_FATAL("Renderer backend create failed!");
        return nullptr;
    }
}