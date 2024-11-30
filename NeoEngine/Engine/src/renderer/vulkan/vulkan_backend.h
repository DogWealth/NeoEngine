//
// Created by NeoLiu on 2024/11/30.
//

#ifndef NEOENGINE_VULKAN_BACKEND_H
#define NEOENGINE_VULKAN_BACKEND_H

#include "renderer/renderer_backend.h"
#include "vulkan_types.h"
namespace NeoEngine {
    class VulkanBackend : public RendererBackend {
    public:
        bool Initialize(const char* application_name) override;
        void Shutdown() override;
        void OnResized(uint16_t width, uint16_t height) override;
        bool BeginFrame(float delta_time) override;
        bool EndFrame(float delta_time) override;

    private:
        VulkanContext context{};
    };


}


#endif //NEOENGINE_VULKAN_BACKEND_H
