#include "StateFunctions.h"
#include "GlobalConstants.h"
#include "GameVariables.h"

#include <iostream>

int daBoxDeath(Game& game) {
    int nextState = GAMEOVER_STATE;

/// UPDATE
    game.daBox.outRect.x--;
    for (int i = 0; i < 16; i++) {
        if (i%2) {
            game.daBox.outRect.x += 3;
        }
        else {
            game.daBox.outRect.x -= 3;
        }

        SDL_RenderClear(game.renderer);

        for (unsigned int i = 0; i < game.daMissiles.size(); i++)
            renderMissile(game.renderer, game.daMissiles[i]);
        for (unsigned int i = 0; i < game.daEnemies.size(); i++)
            renderEnemy(game.renderer, game.daEnemies[i]);
        for (unsigned int i = 0; i < game.daBlasts.size(); i++)
            renderBlast(game.renderer, game.daBlasts[i]);
        renderBox(game.renderer, game.daBox);

        SDL_UpdateWindowSurface(game.window);

        SDL_Delay(40);
    }

    BlastLogic blast;
    initBlast(blast, game.blastSprite, game.daBox.xPosition, game.daBox.yPosition);

    for (int i = 0; i < 50; i++) {
        SDL_RenderClear(game.renderer);

        for (unsigned int i = 0; i < game.daMissiles.size(); i++)
            renderMissile(game.renderer, game.daMissiles[i]);
        for (unsigned int i = 0; i < game.daEnemies.size(); i++)
            renderEnemy(game.renderer, game.daEnemies[i]);
        for (unsigned int i = 0; i < game.daBlasts.size(); i++)
            renderBlast(game.renderer, game.daBlasts[i]);

        if (blast.alive) {
            renderBlast(game.renderer, blast);
            updateBlast(blast);
        }

        SDL_UpdateWindowSurface(game.window);

        SDL_Delay(40);
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
        initBox(game.daBox, game.boxSprite);
        game.daEnemies.clear();
        game.daMissiles.clear();
        game.daBlasts.clear();
        game.lag = 0;
        game.lastTime = SDL_GetTicks();
    }

    return nextState;
}
