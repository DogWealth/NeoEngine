//
// Created by NeoLiu on 2024/10/30.
//

#include "game_test.h"

bool GameTest::Initialize() {
    NEO_INFO("Initializing...");
    return true;
}

bool GameTest::Update(float delta_time) {
    NEO_INFO("Updating...");
    return true;
}

bool GameTest::Render(float delta_time) {
    NEO_INFO("Rendering...");
    return true;
}

void GameTest::OnResize(uint32_t width, uint32_t height) {
}

NeoEngine::Game* NeoEngine::CreateGame() {

    NeoEngine::ApplicationConfig config;
    config.start_pos_x = 100;
    config.start_pos_y = 100;
    config.start_width = 1280;
    config.start_height = 720;
    config.name = "NeoEngine";

    NeoEngine::Game* game = new GameTest;
    game->SetAppConfig(config);
    return game;

}
