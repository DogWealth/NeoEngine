//
// Created by NeoLiu on 2024/11/30.
//

#include "clock.h"
#include "platform/platform.h"
namespace NeoEngine {

    void Clock::Update() {
        if(start_time_ != 0)
            elapse_ = Platform::GetPlatform().GetAbsoluteTime() - start_time_;
    }

    void Clock::Start() {
        start_time_ = Platform::GetPlatform().GetAbsoluteTime();
        elapse_ = 0;
    }

    void Clock::Stop() {
        start_time_ = 0;
    }
}