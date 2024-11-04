//
// Created by NeoLiu on 2024/10/28.
//

#include "application.h"
#include "game.h"
#include "memory.h"
#include "event.h"

namespace NeoEngine {
    Application::Application() : initialized_(false) , state_(){
        state_.platform = &Platform::GetPlatform();
    }

    Application::~Application() {
        if(Event::GetEventSystem().IsInitialized()) {
            Event::GetEventSystem().Shutdown();
            NEO_DEBUG("Event System shutdown");
        }
        state_.platform->Shutdown();
        delete state_.platform;

        NEO_DEBUG("Application has been destroyed");
    }

    bool Application::Create(Game* game) {
        if(initialized_) {
            NEO_ERROR("Application has been created, do not create more than once.");
            return false;
        }

        NEO_DEBUG("Application is created.");

        if(!Event::GetEventSystem().IsInitialized()) {
            Event::GetEventSystem().Initialize();
            NEO_DEBUG("Event system is initialized.");
        }

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
        NEO_INFO(Memory::GetMemoryUsageStr().c_str());

        while(state_.is_running) {
            if(!state_.platform->PumpMessage()) {
                state_.is_running = false;
            }

            if(!state_.is_suspended) {
                if(!state_.game->Update(0)) {
                    NEO_FATAL("Game failed to update.");
                    state_.is_running = false;
                    break;
                }
            }
        }

        state_.is_running = false;
        return true;
    }
}
