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
    SDL_Texture* boxSprite = loadBitmap("assets/daBox.bmp", renderer);
    SDL_Texture* missileSprite = loadBitmap("assets/missile.bmp", renderer);
    SDL_Texture* enemySprite = loadBitmap("assets/evilBox.bmp", renderer);
    SDL_Texture* blastSprite = loadBitmap("assets/blast.bmp", renderer);

/// RESOURCES INIT

    vector<MissileLogic> daMissiles;
    vector<EnemyLogic> daEnemies;
    vector<BlastLogic> daBlasts;

    BoxLogic daBox;
    initBox (daBox, boxSprite);

    unsigned int lastShootDate = 0;
    bool firing = false;

    unsigned int lastSpawnDate = 0;


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

        if (lastSpawnDate + SPAWNING_DELAY < SDL_GetTicks()) {
            spawnPack(enemySprite, daEnemies);
            lastSpawnDate = SDL_GetTicks();
        }

        while(lag >= SCREEN_TPF) {

            updateBox(daBox);

            /// Blasts update
            for (unsigned int i = 0; i < daBlasts.size(); i++) {
                if (daBlasts[i].alive) {
                    updateBlast(daBlasts[i]);
                }
                else {
                    daBlasts.erase(daBlasts.begin()+i);
                }

            }

            /// Missiles update
            for (unsigned int i = 0; i < daMissiles.size(); i++) {

                if (daMissiles[i].alive) {
                    updateMissile(daMissiles[i]);;
                    unsigned int j = 0;
                    while(j < daEnemies.size() && daMissiles[i].alive) {
                        if (collide(daEnemies[j], daMissiles[i])) {
                            BlastLogic blast;
                            initBlast(blast, blastSprite, daEnemies[j].xPosition, daEnemies[j].yPosition);
                            daBlasts.push_back(blast);
                        }
                        j++;
                    }
                }

                else {
                    daMissiles.erase(daMissiles.begin()+i);
                }
            }

            /// Enemies update
            for (unsigned int i = 0; i < daEnemies.size(); i++) {

                if (daEnemies[i].alive) {
                    updateEnemy(daEnemies[i], daBox);
                    for (unsigned int j = i+1; j < daEnemies.size(); j++) {
                        if (i != j) {
                            collide(daEnemies[i], daEnemies[j]);
                        }
                    }
                    collide(daBox, daEnemies[i]);
                }

                else {
                    daEnemies.erase(daEnemies.begin()+i);
                }
            }


        // ..Aaaaand update the lag counter
            lag -= SCREEN_TPF;
        }

        if (!daBox.alive) {
            //Switch to gameover state
        }

/// RENDER

        //cout << daBox.xVelocity << "; " << daBox.yVelocity << endl;
        SDL_RenderClear(renderer);

        for (unsigned int i = 0; i < daMissiles.size(); i++)
            renderMissile(renderer, daMissiles[i]);
        for (unsigned int i = 0; i < daEnemies.size(); i++)
            renderEnemy(renderer, daEnemies[i]);
        for (unsigned int i = 0; i < daBlasts.size(); i++)
            renderBlast(renderer, daBlasts[i]);
        renderBox(renderer, daBox);

        SDL_UpdateWindowSurface(window);
    }

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
