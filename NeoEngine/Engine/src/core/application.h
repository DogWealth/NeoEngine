//
// Created by NeoLiu on 2024/10/28.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include "platform/platform.h"
#include "event.h"
#include "clock.h"

namespace NeoEngine {

    class Game;

    struct ApplicationConfig {
        std::string name;
        int16_t start_pos_x;
        int16_t start_pos_y;
        int16_t start_height;
        int16_t start_width;
    };

    struct ApplicationState {
        Game* game;
        bool is_running;
        bool is_suspended;
        Platform* platform;
        int16_t width, height;
        Clock clock;
        double last_update_time;
    };

    /*
     * at first, i planned to use the singleton, but found that dynamic link would make Application singleton
     * have more than noe instance, so i decided to use static class. it worked
     */
    class NEO_API Application {
    public:
        Application() = default;
        ~Application() = default;

        static bool Create(Game* game);
        static bool Run();

        static void Initialize();
        static void Shutdown();

    public:
        static bool initialized_;
        static ApplicationState state_;
    };
}



#endif //APPLICATION_H
