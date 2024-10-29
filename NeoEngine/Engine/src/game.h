//
// Created by NeoLiu on 2024/10/29.
//

#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <cstdint>
#include "core/application.h"

namespace NeoEngine {
    struct ApplicationConfig;

    class NEO_API Game {
        public:
        explicit Game() = default;
        virtual ~Game() = default;

        virtual bool Initialize() = 0;
        virtual bool Update(float delta_time) = 0;
        virtual bool Render(float delta_time) = 0;
        virtual void OnResize(uint32_t width, uint32_t height) = 0;

        void SetAppConfig(const ApplicationConfig& app_config) {app_config_ = app_config;};

        [[nodiscard]] const ApplicationConfig& GetAppConfig()const {return app_config_;};
        void SetAppConfig(ApplicationConfig&& app_config) {app_config_ = app_config;};

        void* GetGameState() {return state_;};

    protected:
        ApplicationConfig app_config_;
        void* state_;
    };

    //Create a specific game depend on the derived game class
    Game* CreateGame();
}

#endif //GAME_TYPES_H
