//
// Created by NeoLiu on 2024/10/30.
//

#ifndef GAME_TEST_H
#define GAME_TEST_H
#include "game.h"

struct GameState {
    float delta_time;
};

class GameTest : public NeoEngine::Game{
public:
    bool Initialize() override;
    bool Update(float delta_time) override;
    bool Render(float delta_time) override;
    void OnResize(uint32_t width, uint32_t height) override;

};





#endif //GAME_TEST_H
