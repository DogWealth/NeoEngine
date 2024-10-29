//
// Created by NeoLiu on 2024/10/28.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include "platform/platform.h"

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
        double last_update_time;
    };

    class NEO_API Application {
    public:
        Application();
        virtual ~Application();
        bool Create(Game* game);
        bool Run();

    private:
        bool initialized_;
        ApplicationState state_;
    };
}



#endif //APPLICATION_H
