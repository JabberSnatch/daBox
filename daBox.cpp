#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "SDL.h"
#include "BoxLogic.h"
#include "MissileLogic.h"
#include "GlobalConstants.h"

using namespace std;

// TODO (Samu#1#): Add some missiles and ennemies


int main(int argc, char **argv) {

/// SDL INIT
    SDL_Window *window;
    SDL_Surface *screen;
    SDL_Renderer *renderer;

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
    /// boxSprite
    SDL_Surface * tmp = SDL_LoadBMP("assets/daBox.bmp");
    if (!tmp) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bitmap loading fail : %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    SDL_Texture * boxSprite = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);

    /// missileSprite
    tmp = SDL_LoadBMP("assets/missile.bmp");
    if (!tmp) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bitmap loading fail : %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    SDL_Texture * missileSprite = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);

    /// enemySprite
    tmp = SDL_LoadBMP("assets/evilBox.bmp");
    if (!tmp) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bitmap loading fail : %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    SDL_Texture * enemySprite = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);

/// RESOURCES INIT

    BoxLogic protoEnemy;
    initBox (protoEnemy, enemySprite, 50.0f, 30.0f);

    vector<MissileLogic*> daMissiles;

    BoxLogic daBox;
    initBox (daBox, boxSprite);

    unsigned int lastShootDate = 0;
    bool firing = false;


/// GAME LOOP INIT

    bool running = true;
    SDL_Event e;
    unsigned int lastTime = SDL_GetTicks(), currentTime, lag(0), elapsed; // FPS capping variables

    while (running) {

        currentTime = SDL_GetTicks();
        elapsed = currentTime - lastTime;
        lastTime = currentTime;
        lag += elapsed;

        //cout << currentTime << "; " << elapsed << "; " << lag << endl;

/// HANDLE INPUTS

        SDL_PollEvent(&e);
        //cout << "Polling event.." << endl;

        if (e.type == SDL_QUIT) {
            running = false;
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {

            case SDLK_z:
                daBox.directions[0] = 1;
                break;
            case SDLK_d:
                daBox.directions[1] = 1;
                break;
            case SDLK_s:
                daBox.directions[2] = 1;
                break;
            case SDLK_q:
                daBox.directions[3] = 1;
                break;

            case SDLK_UP:
                //cout << "UPd; ";
                daBox.orientation = 0;
                break;
            case SDLK_RIGHT:
                //cout << "RIGHTd; ";
                daBox.orientation = 1;
                break;
            case SDLK_DOWN:
                //cout << "DOWNd; ";
                daBox.orientation = 2;
                break;
            case SDLK_LEFT:
                //cout << "LEFTd; ";
                daBox.orientation = 3;
                break;

            case SDLK_SPACE:
                firing = true;
                break;

            case SDLK_ESCAPE:
                running = false;
                break;
            }
        }
        if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
            case SDLK_z:
                //cout << "UPu; ";
                daBox.directions[0] = 0;
                break;
            case SDLK_d:
                //cout << "RIGHTu; ";
                daBox.directions[1] = 0;
                break;
            case SDLK_s:
                //cout << "DOWNu; ";
                daBox.directions[2] = 0;
                break;
            case SDLK_q:
                //cout << "LEFTu; ";
                daBox.directions[3] = 0;
                break;

            case SDLK_SPACE:
                firing = false;
                break;
            }
        }

/// UPDATE
        if (firing && lastShootDate + FIRING_DELAY < SDL_GetTicks()) {
            daMissiles.push_back(fireMissile(daBox, missileSprite));
            lastShootDate = SDL_GetTicks();
        }
        while(lag >= SCREEN_TPF) {

            updateBox(daBox);

            for (unsigned int i = 0; i < daMissiles.size(); i++) {
                if (daMissiles[i]->alive)
                    updateMissile(daMissiles[i]);
                else {
                    delete daMissiles[i];
                    daMissiles.erase(daMissiles.begin()+i);
                }
            }

        // ..Aaaaand update the lag counter
            lag -= SCREEN_TPF;
        }

/// RENDER

        //cout << daBox.xVelocity << "; " << daBox.yVelocity << endl;
        SDL_RenderClear(renderer);

        for (unsigned int i = 0; i < daMissiles.size(); i++)
            renderMissile(renderer, daMissiles[i]);
        renderBox(renderer, daBox);
        renderBox(renderer, protoEnemy);

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyTexture(boxSprite);
    boxSprite = NULL;
    SDL_DestroyTexture(missileSprite);
    missileSprite = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    return 0;
}
