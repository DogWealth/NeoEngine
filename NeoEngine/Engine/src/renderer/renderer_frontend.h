//
// Created by NeoLiu on 2024/11/30.
//

#ifndef NEOENGINE_RENDERER_FRONTEND_H
#define NEOENGINE_RENDERER_FRONTEND_H
#include "renderer_backend.h"
namespace NeoEngine {

    class NEO_API RendererFrontend {
    public:
        static bool Initialize(const char* application_name);
        static void Shutdown();
        static void OnResized(uint16_t width, uint16_t height);
        static bool BeginFrame(float  delta_time);
        static bool EndFrame(float delta_time);
        static bool DrawFrame(RenderPacket* packet);

    private:
        static RendererBackend* backend;
    };
} // NeoEngine

#endif //NEOENGINE_RENDERER_FRONTEND_H
