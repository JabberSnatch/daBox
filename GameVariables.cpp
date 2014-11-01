#include "GameVariables.h"
#include "GlobalConstants.h"

#include "SDL_ttf.h"

#include <iostream>
#include <string>
#include <cstdlib>

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

// TODO: find a way to get rid of the unexpected segfault in this function
void renderScore(Game& game) {
    //std::cout << game.score << "; x" << game.multiplier << std::endl;

    SDL_Color color; color.r = 255; color.b = 255; color.g = 255;
    SDL_Rect outRect;
    SDL_Texture* textTexture;
    SDL_Surface* textSurface;

/// RENDER SCORE
    if (game.score > 999999999999) { // This guard prevents itoa to overflow
        game.score = 0;
    }
    char score [12];
    itoa(game.score, score, 10);
    //std::string score = std::to_string(game.score); // Doesn't work, the problem seems to come from MinGW

    std::cout << (game.scoreFont != NULL) << "; " << score << std::endl;
    textSurface = TTF_RenderText_Solid(game.scoreFont, score, color);
    if (textSurface == NULL) {
        std::cout << TTF_GetError() << std::endl;
    }
    else {
        textTexture = SDL_CreateTextureFromSurface(game.renderer, textSurface);
        outRect.w = textSurface->w;
        outRect.h = textSurface->h;
        outRect.y = 5;
        outRect.x = SCREEN_WIDTH - outRect.w - 7;

        SDL_RenderCopy(game.renderer, textTexture, NULL, &outRect);
    }

/// RENDER MULTIPLIER
    char multiplier [12];

    // Formatting game.multiplier to a char*
    int iMul = game.multiplier * 10;
    itoa(iMul, multiplier, 10);
    int i = 0;
    while (multiplier[i] != '\0') {
        i++;
    }
    multiplier[i+2] = '\0';
    multiplier[i+1] = multiplier[i-1];
    multiplier[i--] = ',';
    for (i = i; i > 0; i--) {
        multiplier[i] = multiplier[i-1];
    }
    multiplier[0] = 'x';

    std::cout << (game.scoreFont != NULL) << "; " << multiplier << std::endl;
    textSurface = TTF_RenderText_Solid(game.scoreFont, multiplier, color);
    if (textSurface == NULL) {
        std::cout << TTF_GetError() << std::endl;
    }
    else {
        textTexture = SDL_CreateTextureFromSurface(game.renderer, textSurface);
        outRect.y = outRect.y + outRect.h + 5;
        outRect.w = textSurface->w;
        outRect.h = textSurface->h;
        outRect.x = SCREEN_WIDTH - outRect.w - 7;

        SDL_RenderCopy(game.renderer, textTexture, NULL, &outRect);
    }

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

