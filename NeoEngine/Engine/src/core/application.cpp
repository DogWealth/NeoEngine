//
// Created by NeoLiu on 2024/10/28.
//

#include "application.h"
#include "game.h"
#include "memory.h"
#include "event.h"
#include "input.h"
#include "renderer/renderer_frontend.h"

namespace NeoEngine {
    bool OnEvent(EventType event_type, void* sender, void* listener, EventContext context);
    bool OnKey(EventType event_type, void* sender, void* listener, EventContext context);

    bool Application::initialized_ = false;
    ApplicationState Application::state_{};

    bool Application::Create(Game* game) {
        if(initialized_) {
            NEO_ERROR("Application has been created, do not create more than once.");
            return false;
        }

        NEO_DEBUG_MSG("Application is created.");
        //initialize application
        Initialize();

        //initialize input system
        Input::GetInputSystem().Initialize();

        //initialize event system
        if(!Event::GetEventSystem().IsInitialized()) {
            Event::GetEventSystem().Initialize();
        }

        Event::GetEventSystem().EventRegister(EventType::APPLICATION_QUIT, nullptr, OnEvent);
        Event::GetEventSystem().EventRegister(EventType::KEY_PRESSED, nullptr, OnKey);
        Event::GetEventSystem().EventRegister(EventType::KEY_RELEASED, nullptr, OnKey);

        state_.game = game;
        state_.is_running = true;
        state_.is_suspended = false;

        if(!state_.platform->Startup(
            game->GetAppConfig().name,
            game->GetAppConfig().start_pos_x,
            game->GetAppConfig().start_pos_y,
            game->GetAppConfig().start_width,
            game->GetAppConfig().start_height)) {
            return false;
        }

        //Renderer initialize
        if(!RendererFrontend::Initialize(game->GetAppConfig().name.c_str())) {
            NEO_FATAL("Renderer failed to initialize.");
            return false;
        }

        //initialize game
        if(!state_.game->Initialize()) {
            NEO_FATAL("Game failed to initialize.");
            return false;
        }
        initialized_ = true;

        //resize game
        state_.game->OnResize(state_.width, state_.height);

        return true;
    }

    bool Application::Run() {
        //process clock && time
        state_.clock.Start();
        state_.clock.Update();
        state_.last_update_time = state_.clock.GetElapse();
        double running_time = 0;
        uint8_t  frame_count = 0;
        double target_frame_seconds = 1.0f / 60;

        NEO_INFO(Memory::GetMemoryUsageStr());

        while(state_.is_running) {
            if(!state_.platform->PumpMessage()) {
                state_.is_running = false;
            }

            if(!state_.is_suspended) {
                //Update clock and get delta time
                state_.clock.Update();
                double current_time = state_.clock.GetElapse();
                double delta = current_time - state_.last_update_time;
                double frame_start_time = Platform::GetPlatform().GetAbsoluteTime();

                if(!state_.game->Update(static_cast<float>(delta))) {
                    NEO_FATAL("Game failed to update.");
                    state_.is_running = false;
                    break;
                }

                //Render
                if(!state_.game->Render(static_cast<float>(delta))) {
                    NEO_FATAL("Game failed to render.");
                    state_.is_running = false;
                    break;
                }

                //TODO:need to refactor packet creation
                RenderPacket packet{};
                packet.delta_time = static_cast<float>(delta);
                if(!RendererFrontend::DrawFrame(&packet)) {
                    state_.is_running = false;
                    break;
                }

                //figure out how long the frame took and, if below
                double frame_end_time = Platform::GetPlatform().GetAbsoluteTime();
                double frame_elapsed_time = frame_end_time - frame_start_time;
                running_time += frame_elapsed_time;
                double remaining_seconds = target_frame_seconds - frame_elapsed_time;

                if(remaining_seconds > 0) {
                    double remaining_ms = remaining_seconds * 1000;

                    bool limit_frames = false;
                    if(remaining_ms > 0 && limit_frames) {
                        Platform::GetPlatform().Sleep_(static_cast<uint64_t>(remaining_ms)- 1);
                    }

                    frame_count++;
                }
                // NOTE: Input update/state copying should always be handled
                //after any input should be recorded; I.E. before this line.
                //AS a safety, input is the last thing to be updated before this frame ends.
                Input::GetInputSystem().Update(static_cast<float>(delta));

                //update last time
                state_.last_update_time = current_time;
            }
        }

        state_.is_running = false;

        Event::GetEventSystem().EventUnregister(EventType::APPLICATION_QUIT, nullptr, OnEvent);
        Event::GetEventSystem().EventUnregister(EventType::KEY_PRESSED, nullptr, OnKey);
        Event::GetEventSystem().EventUnregister(EventType::KEY_RELEASED, nullptr, OnKey);

        Shutdown();

        return true;
    }

    void Application::Shutdown() {
        if(Event::GetEventSystem().IsInitialized()) {
            Event::GetEventSystem().Shutdown();
            NEO_DEBUG_MSG("Event System shutdown");
        }
        Input::GetInputSystem().Shutdown();
        NEO_DEBUG_MSG("Input System shutdown");

        RendererFrontend::Shutdown();
        NEO_DEBUG_MSG("Render System shutdown");

        state_.platform->Shutdown();
        //delete state_.platform;

        NEO_DEBUG_MSG("Application has been destroyed");
    }

    void Application::Initialize() {
        state_.platform = &Platform::GetPlatform();
    }

    bool OnEvent(EventType event_type, void *sender, void *listener, EventContext context) {
        switch (event_type) {
            case EventType::APPLICATION_QUIT: {
                NEO_INFO("Application quit");
                Application::state_.is_running = false;
                return true;
            }
            case EventType::KEY_PRESSED:
                break;
            case EventType::KEY_RELEASED:
                break;
            case EventType::BUTTON_PRESSED:
                break;
            case EventType::BUTTON_RELEASED:
                break;
            case EventType::MOUSE_MOVED:
                break;
            case EventType::MOUSE_WHEEL:
                break;
            case EventType::RESIZED:
                break;
            case EventType::MAX_EVENT_CODE:
                break;
        }
        return false;
    }

    bool OnKey(EventType event_type, void *sender, void *listener, EventContext context) {
        if(event_type == EventType::KEY_PRESSED) {
            uint16_t key = context.data.u16[0];

            if(static_cast<Keys>(key) == Keys::KEY_ESCAPE) {
                EventContext data{};
                Event::GetEventSystem().EventFire(EventType::APPLICATION_QUIT, nullptr, data);

                return true;
            } else if(static_cast<Keys>(key) == Keys::KEY_A) {
                NEO_DEBUG_MSG("Explicit - A Key Pressed");
            } else {
                NEO_DEBUG_MSG("'%c' Key Pressed", static_cast<Keys>(key));
            }
        } else if (event_type == EventType::KEY_RELEASED) {
            uint16_t key = context.data.u16[0];
            if(static_cast<Keys>(key) == Keys::KEY_B) {
                NEO_DEBUG_MSG("Explicit - B Key Released");
            } else {
                NEO_DEBUG_MSG("'%c' Key Released", static_cast<Keys>(key));
            }
        }
        return false;
    }
}
