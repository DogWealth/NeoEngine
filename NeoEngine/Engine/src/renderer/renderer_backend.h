//
// Created by NeoLiu on 2024/11/30.
//

#ifndef NEOENGINE_RENDERER_BACKEND_H
#define NEOENGINE_RENDERER_BACKEND_H
#include "defines.h"
#include <cstdint>
namespace NeoEngine {
    enum class RendererBackendType {
        Renderer_BACKEND_VULKAN,
        Renderer_BACKEND_DIRECTX,
    };

    struct  RenderPacket {
        float delta_time;
    };

    class NEO_API RendererBackend {
    public:
        RendererBackend() : frame_number_(0) {}
        virtual bool Initialize(const char* application_name) = 0;
        virtual void Shutdown() = 0;
        virtual void OnResized(uint16_t width, uint16_t height) = 0;
        virtual bool BeginFrame(float delta_time) = 0;
        virtual bool EndFrame(float delta_time) = 0;

        static RendererBackend* Create(RendererBackendType type);

    public:
        double frame_number_;
    };
}
#endif //NEOENGINE_RENDERER_BACKEND_H
