#include "StateFunctions.h"
#include "GlobalConstants.h"
#include "GameVariables.h"

#include <iostream>

int daBoxDeath(Game& game) {
    int nextState = CORE_STATE;

/// UPDATE
    game.daBox.outRect.x--;
    for (int i = 0; i < 16; i++) {
        if (i%2) {
            game.daBox.outRect.x += 3;
        }
        else {
            game.daBox.outRect.x -= 3;
        }

        renderAll(game);

        SDL_Delay(40);
    }

    game.daBox.alive = false;
    BlastLogic blast;
    initBlast(blast, game.blastSprite, game.daBox.xPosition, game.daBox.yPosition);

    for (int i = 0; i < 50; i++) {
        SDL_RenderClear(game.renderer);

        renderAll(game);

        if (blast.alive) {
            renderBlast(game.renderer, blast);
            updateBlast(blast);
        }

        for (unsigned int i = 0; i < game.lives.size(); i++)
            renderBlast(game.renderer, game.lives[i]);

        SDL_UpdateWindowSurface(game.window);

        SDL_Delay(40);
    }

    if (game.lives.size() <= 0) {
        nextState = GAMEOVER_STATE;
    }
    else {
        game.daBox.alive = true;

        game.daEnemies.clear();
        game.daMissiles.clear();
        game.daBlasts.clear();

        game.lastSpawnDate = 0;
        game.lastTime = SDL_GetTicks();
        game.lag = 0;
    }

    return nextState;
}

int gameOverState(Game& game) {
    int nextState = GAMEOVER_STATE;

    SDL_RenderCopy(game.renderer, game.gameoverScreen, NULL, NULL);
    SDL_UpdateWindowSurface(game.window);

    if (game.e.type == SDL_QUIT) {
        game.running = false;
    }
    if (game.e.type == SDL_KEYDOWN) {
        switch (game.e.key.keysym.sym) {
        case SDLK_ESCAPE:
            game.running = false;
            break;
        case SDLK_SPACE:
            nextState = CORE_STATE;
            break;
        }
    }

    // Handle the area cleanup
    if (nextState == CORE_STATE) {
        resetGame(game);
    }

    return nextState;
}
