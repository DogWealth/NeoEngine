//
// Created by NeoLiu on 2024/11/30.
//

#ifndef NEOENGINE_CLOCK_H
#define NEOENGINE_CLOCK_H


namespace NeoEngine {
    class Clock {
    public:
        Clock() : start_time_(0), elapse_(0) {};
        ~Clock() = default;

        void Update();
        void Start();
        void Stop();

        double GetElapse() const { return elapse_; }
        double GetStartTime() const { return start_time_; }

    private:
        double start_time_;
        double elapse_;
    };
}


#endif //NEOENGINE_CLOCK_H
