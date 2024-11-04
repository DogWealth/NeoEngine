//
// Created by NeoLiu on 2024/11/2.
//

#ifndef EVENT_H
#define EVENT_H
#include <functional>
#include "containers/darray.h"

namespace NeoEngine {

#define MAX_MESSAGE_CODES 16384

    struct EventContext {
      union {
          int64_t   i64[2];
          uint64_t  u64[2];
          double    f64[2];

          int32_t   i32[4];
          uint32_t  u32[4];
          float     f32[4];

          int16_t   i16[8];
          uint16_t  u16[8];

          char      c[16];
      } data;
    };

    //System internal event code. Application should use codes beyond 255
    enum class EventType {
        APPLICATION_QUIT    = 0x01,
        KEY_PRESSED         = 0x02,
        KEY_RELEASED        = 0x03,
        BUTTON_PRESSED      = 0x04,
        BUTTON_RELEASED     = 0x05,
        MOUSE_MOVED         = 0x06,
        MOUSE_WHEEL         = 0x07,
        RESIZED             = 0x08,

        MAX_EVENT_CODE      = 0xFF

    };

    class NEO_API Event {
    public:
        Event(Event& event) = delete;
        Event(Event&& event) = delete;
        Event& operator=(Event& event) = delete;
        Event& operator=(Event&& event) = delete;
        static Event& GetEventSystem() {
            static Event event;
            return event;
        }

        using PFN_On_EventCallback = bool(*)(EventType event_type, void* sender, void* listener_inst, EventContext data);

        bool Initialize();
        void Shutdown();

        bool EventRegister(EventType event_type, void* listener, PFN_On_EventCallback on_event);
        bool EventUnregister(EventType event_type, void* listener, PFN_On_EventCallback on_event);
        bool EventFire(EventType event_type, void* sender, EventContext context);

        [[nodiscard]] bool IsInitialized() const {return is_initialized_;};

    private:
        Event() = default;

    private:
        struct RegisteredEvent {
            void* listener;
            PFN_On_EventCallback callback;
        };

        struct EventCodeEntry {
            DArray<RegisteredEvent> events;
        };

        struct EventSystemState {
            EventCodeEntry registered[MAX_MESSAGE_CODES];
        };

        bool is_initialized_ = false;
        EventSystemState state_;
    };
}



#endif //EVENT_H
