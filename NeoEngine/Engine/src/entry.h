//
// Created by NeoLiu on 2024/10/29.
//

#ifndef ENTRY_H
#define ENTRY_H

#include "core/application.h"
#include "core/logger.h"
#include "game.h"

//main entry point
int main() {
    NeoEngine::Game* game = NeoEngine::CreateGame();
    if(game == nullptr) {
        NEO_FATAL("Couldn't create game.\n");
        return -1;
    }

    NeoEngine::Application app;
    if(!app.Create(game)) {
        NEO_FATAL("Couldn't create application.\n");
        return 1;
    }

    if(!app.Run()) {
        NEO_FATAL("Application did not shutdown gracefully.\n");
        return 2;
    }
}

#endif //ENTRY_H
