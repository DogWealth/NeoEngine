//
// Created by NeoLiu on 2024/11/30.
//

#include "renderer_frontend.h"
namespace NeoEngine {
    RendererBackend* RendererFrontend::backend = nullptr;

    bool RendererFrontend::Initialize(const char *application_name) {
        backend = RendererBackend::Create(RendererBackendType::Renderer_BACKEND_DIRECTX);
        backend->frame_number_ = 0;

        if(!backend->Initialize(application_name)) {
            NEO_FATAL("Renderer backend failed to initialize!");
            return false;
        }

        return true;
    }

    void RendererFrontend::Shutdown() {
        backend->Shutdown();
    }

    void RendererFrontend::OnResized(uint16_t width, uint16_t height) {

    }

    bool RendererFrontend::BeginFrame(float delta_time) {
        return backend->BeginFrame(delta_time);
    }

    bool RendererFrontend::EndFrame(float delta_time) {
        bool result = backend->EndFrame(delta_time);
        backend->frame_number_++;
        return result;
    }

    bool RendererFrontend::DrawFrame(RenderPacket *packet) {
        if(!backend->BeginFrame(packet->delta_time)) {
            bool result = backend->EndFrame(packet->delta_time);

            if(!result) {
                NEO_ERROR("End Frame failed. Application Shut down!");
                return false;
            }
        }
        return true;
    }

} // NeoEngine