//
// Created by NeoLiu on 2024/10/29.
//

#ifndef ENTRY_H
#define ENTRY_H

#include "core/application.h"
#include "core/logger.h"
#include "game.h"
#include "core/memory.h"

//main entry point
int main() {
    NeoEngine::Memory::Initialize();

    NeoEngine::Game* game = NeoEngine::CreateGame();
    if(game == nullptr) {
        NEO_FATAL("Couldn't create game.\n");
        return -1;
    }

    if(!NeoEngine::Application::Create(game)) {
        NEO_FATAL("Couldn't create application.\n");
        return 1;
    }

    if(!NeoEngine::Application::Run()) {
        NEO_FATAL("Application did not shutdown gracefully.\n");
        return 2;
    }

    NeoEngine::Memory::Destroy();
}

#endif //ENTRY_H
