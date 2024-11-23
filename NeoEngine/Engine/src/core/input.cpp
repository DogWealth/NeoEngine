//
// Created by NeoLiu on 2024/11/23.
//

#include "input.h"
#include "memory.h"
#include "event.h"
#include "logger.h"
namespace NeoEngine {
    void Input::Initialize() {
        initialized_ = true;
        NEO_DEBUG_MSG("Input subsystem initialized");
    }

    void Input::Shutdown() {
        initialized_ = false;
    }

    void Input::Update(double delta_time) {
        if(!initialized_)
            return;

        Memory::Copy_Memory(&state_.keyboard_previous_, &state_.keyboard_current_, sizeof(KeyboardState));
        Memory::Copy_Memory(&state_.mouse_previous_, &state_.mouse_current_, sizeof(KeyboardState));
    }

    bool Input::IsKeyDown(Keys key) {
        if(!initialized_)
            return false;

        return state_.keyboard_current_.keys[static_cast<uint16_t>(key)];
    }

    bool Input::IsKeyUp(Keys key) {
        if(!initialized_)
            return false;

        return !state_.keyboard_current_.keys[static_cast<uint16_t>(key)];
    }

    bool Input::WasKeyDown(Keys key) {
        if(!initialized_)
            return false;

        return state_.keyboard_previous_.keys[static_cast<uint16_t>(key)];
    }

    bool Input::WasKeyUp(Keys key) {
        if(!initialized_)
            return false;

        return !state_.keyboard_previous_.keys[static_cast<uint16_t>(key)];
    }

    void Input::ProcessKey(Keys key, bool pressed) {
        if(state_.keyboard_current_.keys[static_cast<uint16_t>(key)] != pressed) {
            state_.keyboard_current_.keys[static_cast<uint16_t>(key)] = pressed;

            EventContext context{};
            context.data.u16[0] = static_cast<uint16_t>(key);
            Event::GetEventSystem().EventFire(pressed ? EventType::KEY_PRESSED : EventType::KEY_RELEASED, nullptr, context);
        }
    }

    bool Input::IsButtonDown(Keys key) {
        if(!initialized_)
            return false;

        return state_.mouse_current_.buttons_[static_cast<uint16_t>(key)];
    }

    bool Input::IsButtonUp(Keys key) {
        if(!initialized_)
            return false;

        return !state_.mouse_current_.buttons_[static_cast<uint16_t>(key)];
    }

    bool Input::WasButtonDown(Keys key) {
        if(!initialized_)
            return false;

        return state_.mouse_previous_.buttons_[static_cast<uint16_t>(key)];
    }

    bool Input::WasButtonUp(Keys key) {
        if(!initialized_)
            return false;

        return !state_.mouse_previous_.buttons_[static_cast<uint16_t>(key)];
    }

    void Input::GetMousePosition(int32_t *x, int32_t *y) const {
        if(!initialized_) {
            *x = 0;
            *y = 0;
            return;
        }

        *x = state_.mouse_current_.x_;
        *y = state_.mouse_current_.y_;
    }

    void Input::GetPreMousePosition(int32_t *x, int32_t *y) const {
        if(!initialized_) {
            *x = 0;
            *y = 0;
            return;
        }

        *x = state_.mouse_previous_.x_;
        *y = state_.mouse_previous_.y_;
    }

    void Input::ProcessButton(MouseButtons button, bool pressed) {
        if(state_.keyboard_current_.keys[static_cast<uint16_t>(button)] != pressed) {
            state_.keyboard_current_.keys[static_cast<uint16_t>(button)] = pressed;

            EventContext context{};
            context.data.u16[0] = static_cast<uint16_t>(button);
            Event::GetEventSystem().EventFire(pressed ? EventType::BUTTON_PRESSED : EventType::BUTTON_RELEASED, nullptr, context);
        }
    }

    void Input::ProcessMouseMove(int16_t x, int16_t y) {
        if(state_.mouse_current_.x_ != x || state_.mouse_current_.y_ != y) {
            state_.mouse_current_.x_ = x;
            state_.mouse_current_.y_ = y;

            //NEO_DEBUG_MSG("Mouse Pos: %i, %i", x, y);

            EventContext context{};
            context.data.u16[0] = x;
            context.data.u16[1] = y;
            Event::GetEventSystem().EventFire(EventType::MOUSE_MOVED, nullptr, context);
        }
    }

    void Input::ProcessMouseWheel(int8_t z_delta) {
        EventContext context{};
        context.data.u8[0] = z_delta;
        Event::GetEventSystem().EventFire(EventType::MOUSE_WHEEL, nullptr, context);
    }
} // NeoEngine