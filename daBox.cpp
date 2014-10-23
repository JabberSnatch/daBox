#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>

#include "SDL.h"
#include "BoxLogic.h"
#include "EnemyLogic.h"
#include "MissileLogic.h"
#include "BlastLogic.h"
#include "GlobalConstants.h"
#include "GameVariables.h"
#include "stateFunctions.h"

using namespace std;

// TODO (Samu#1#): Add other stuff

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
    int gameState = 0;

/// SDL INIT

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail : %s\n", SDL_GetError());
        return 1;
    }

    game.window = SDL_CreateWindow("daBox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!game.window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation fail : %s\n", SDL_GetError());
        return 1;
    }

    game.screen = SDL_GetWindowSurface(game.window);
    game.renderer = SDL_CreateSoftwareRenderer(game.screen);

/// Sprites loading
    game.boxSprite = loadBitmap("assets/daBox.bmp", game.renderer);
    game.missileSprite = loadBitmap("assets/missile.bmp", game.renderer);
    game.enemySprite = loadBitmap("assets/evilBox.bmp", game.renderer);
    game.blastSprite = loadBitmap("assets/blast.bmp", game.renderer);

/// RESOURCES INIT

    initBox(game.daBox, game.boxSprite);

    game.lastSpawnDate = 0;

    game.running = true;
    // FPS capping variables
    game.lastTime = SDL_GetTicks();
    game.lag = 0;

/// GAME LOOP

    while (game.running) {

        game.currentTime = SDL_GetTicks();
        game.elapsed = game.currentTime - game.lastTime;
        game.lastTime = game.currentTime;
        game.lag += game.elapsed;

        //cout << currentTime << "; " << elapsed << "; " << lag << endl;
        SDL_PollEvent(&game.e);
        switch (gameState) {
        case 0:
            gameState = coreState(game);
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

    SDL_Quit();

    return 0;
}
