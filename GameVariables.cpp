#include "GameVariables.h"
#include "GlobalConstants.h"

#include <iostream>

void resetGame(Game& game) {
    initBox(game.daBox, game.boxSprite);
    initItem(game.bonusItem);

    game.daEnemies.clear();
    game.daMissiles.clear();
    game.daBlasts.clear();
    game.lives.clear();

    for (int i = 0; i < STARTING_LIVES; i++) {
        BlastLogic life;
        initBlast(life, game.heartSprite, 7.0+(i*17.0), 5.0);
        life.outRect.w = life.outRect.h = 16;
        game.lives.push_back(life);
    }

    game.score = 0;
    game.multiplier = 1.0f;
    game.lastSpawnDate = SDL_GetTicks();
    game.lastTime = SDL_GetTicks();
    game.lag = 0;
}

void renderAll(Game& game) {
    SDL_RenderClear(game.renderer);

    if (game.bonusItem.alive)
        renderItem(game.renderer, game.bonusItem);

    for (unsigned int i = 0; i < game.daMissiles.size(); i++)
        renderMissile(game.renderer, game.daMissiles[i]);
    for (unsigned int i = 0; i < game.daEnemies.size(); i++)
        renderEnemy(game.renderer, game.daEnemies[i]);
    for (unsigned int i = 0; i < game.daBlasts.size(); i++)
        renderBlast(game.renderer, game.daBlasts[i]);

    if (game.daBox.alive)
        renderBox(game.renderer, game.daBox);

    for (unsigned int i = 0; i < game.lives.size(); i++)
        renderBlast(game.renderer, game.lives[i]);

    renderScore(game);

    SDL_UpdateWindowSurface(game.window);
}

void renderScore(Game& game) {
    std::cout << game.score << "; x" << game.multiplier << std::endl;
}

