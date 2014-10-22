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

/// SDL INIT

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail : %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("daBox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation fail : %s\n", SDL_GetError());
        return 1;
    }

    screen = SDL_GetWindowSurface(window);
    renderer = SDL_CreateSoftwareRenderer(screen);

/// Sprites loading
    boxSprite = loadBitmap("assets/daBox.bmp", renderer);
    missileSprite = loadBitmap("assets/missile.bmp", renderer);
    enemySprite = loadBitmap("assets/evilBox.bmp", renderer);
    blastSprite = loadBitmap("assets/blast.bmp", renderer);

/// RESOURCES INIT

    initBox (daBox, boxSprite);

    lastShootDate = 0;
    firing = false;
    lastSpawnDate = 0;

    running = true;
    // FPS capping variables
    lastTime = SDL_GetTicks();
    lag = 0;

/// GAME LOOP

    while (running) {

        currentTime = SDL_GetTicks();
        elapsed = currentTime - lastTime;
        lastTime = currentTime;
        lag += elapsed;

        //cout << currentTime << "; " << elapsed << "; " << lag << endl;
        switch (gameState) {
        case 0:
            gameState = coreState();
        }

    }

/// END OF GAME LOOP

    SDL_DestroyTexture(boxSprite);
    SDL_DestroyTexture(missileSprite);
    SDL_DestroyTexture(enemySprite);
    SDL_DestroyTexture(blastSprite);

    boxSprite = NULL;
    missileSprite = NULL;
    enemySprite = NULL;
    blastSprite = NULL;


    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    return 0;
}
