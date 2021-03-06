#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "SDL.h"
#include "SDL_ttf.h"
#include "GlobalConstants.h"
#include "GameVariables.h"
#include "StateFunctions.h"

using namespace std;

// TODO (Samu#1#): Add other stuff
// TODO (Samu#2#): Add a nuke. It should be reloading over time

SDL_Texture* loadBitmap(string path, SDL_Renderer* renderer) {
    SDL_Surface * tmp = SDL_LoadBMP(path.c_str());
    if (!tmp) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bitmap loading fail : %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    SDL_Texture * bmpTexture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);

    return bmpTexture;
}

int main(int argc, char **argv) {

    srand(time(NULL));
    Game game;
    game.state = CORE_STATE;

/// SDL INIT

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail : %s\n", SDL_GetError());
        return 1;
    }
    TTF_Init();

    game.window = SDL_CreateWindow("daBox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!game.window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation fail : %s\n", SDL_GetError());
        return 1;
    }

    game.screen = SDL_GetWindowSurface(game.window);
    game.renderer = SDL_CreateSoftwareRenderer(game.screen);

    game.scoreFont = TTF_OpenFont("assets/scoreFont.ttf", 32);

/// Sprites loading
    game.boxSprite = loadBitmap("assets/daBox.bmp", game.renderer);
    game.missileSprite = loadBitmap("assets/missile.bmp", game.renderer);
    game.enemySprite = loadBitmap("assets/evilBox.bmp", game.renderer);
    game.blastSprite = loadBitmap("assets/blast.bmp", game.renderer);
    game.heartSprite = loadBitmap("assets/heart.bmp", game.renderer);
    game.gameoverScreen = loadBitmap("assets/gameoverScreen.bmp", game.renderer);
    loadItemSprite(game.bonusItem, game.renderer);

/// GAME INIT
    resetGame(game);

/// GAME LOOP
    game.running = true;
    while (game.running) {

        game.currentTime = SDL_GetTicks();
        game.elapsed = game.currentTime - game.lastTime;
        game.lastTime = game.currentTime;
        game.lag += game.elapsed;

        //cout << currentTime << "; " << elapsed << "; " << lag << endl;
        switch (game.state) {
        case CORE_STATE:
            game.state = coreState(game);
            break;
        case DABOXDEATH_STATE:
            game.state = daBoxDeath(game);
            break;
        case GAMEOVER_STATE:
            game.state = gameOverState(game);
            break;
        }
    }

/// END OF GAME LOOP

    SDL_DestroyTexture(game.boxSprite);
    SDL_DestroyTexture(game.missileSprite);
    SDL_DestroyTexture(game.enemySprite);
    SDL_DestroyTexture(game.blastSprite);

    game.boxSprite = NULL;
    game.missileSprite = NULL;
    game.enemySprite = NULL;
    game.blastSprite = NULL;


    SDL_DestroyRenderer(game.renderer);
    game.renderer = NULL;
    SDL_DestroyWindow(game.window);
    game.window = NULL;

    TTF_Quit();
    SDL_Quit();

    return 0;
}
