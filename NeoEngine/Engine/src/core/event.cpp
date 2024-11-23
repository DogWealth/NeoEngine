//
// Created by NeoLiu on 2024/11/2.
//

#include "event.h"

#include "memory.h"

namespace NeoEngine {
    bool Event::Initialize() {
        if(is_initialized_) return false;

        Memory::Zero_Memory(&state_, sizeof(state_));
        is_initialized_ = true;
        NEO_DEBUG_MSG("Event system is initialized.");
        return true;
    }

    void Event::Shutdown() {
        for(int16_t i = 0; i < MAX_MESSAGE_CODES; ++i) {
            if(!state_.registered[i].events.IsEmpty()) {
                state_.registered[i].events.Clear();
            }
        }
    }

    bool Event::EventRegister(EventType event_type, void *listener, PFN_On_EventCallback on_event) {
        if(is_initialized_) {
            const uint64_t register_count = state_.registered[static_cast<uint8_t>(event_type)].events.Size();
            for(int i = 0; i < register_count; ++i) {
                if(state_.registered[static_cast<uint8_t>(event_type)].events[i].listener == listener) {
                    //TODO: Warn
                    return false;
                }
            }

            RegisteredEvent event{};
            event.listener = listener;
            event.callback = on_event;
            state_.registered[static_cast<uint8_t>(event_type)].events.PushBack(event);

            return true;
        }

        return false;
    }

    bool Event::EventUnregister(EventType event_type, void *listener, const PFN_On_EventCallback on_event) {
        if(is_initialized_) {
            if(state_.registered[static_cast<uint8_t>(event_type)].events.IsEmpty()) {
                //TODO:Warn
                return false;
            }

            const uint64_t register_count = state_.registered[static_cast<uint8_t>(event_type)].events.Size();
            for(int i = 0; i < register_count; ++i) {
                const RegisteredEvent& event = state_.registered[static_cast<uint8_t>(event_type)].events[i];
                if(event.listener == listener || event.callback == on_event) {
                    state_.registered[static_cast<uint8_t>(event_type)].events.Erase(i);
                    return true;
                }
            }
        }
        return false;
    }

    bool Event::EventFire(EventType event_type, void *sender, EventContext context) {
        if(is_initialized_) {
            if(state_.registered[static_cast<uint8_t>(event_type)].events.IsEmpty()) {
                //TODO:Warn
                return false;
            }

            const uint64_t register_count = state_.registered[static_cast<uint16_t>(event_type)].events.Size();
            for(int i = 0; i < register_count; ++i) {
                const RegisteredEvent& event = state_.registered[static_cast<uint16_t>(event_type)].events[i];
                if(event.callback(event_type, sender, event.listener, context)) {
                    return true;
                }
            }
        }
        return false;
    }
}
