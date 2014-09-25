#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "SDL.h"

// TODO (Samu#2#): This variable should be removed from here
bool missileFired = false;

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

const unsigned int SCREEN_FPS = 60;
const unsigned int SCREEN_TPF = 1000 / SCREEN_FPS; // TPF = Ticks Per Frame

const double MAX_SPEED = 3.7f;
const double ACC_FACTOR = 0.7f;
const double FRICTION = 0.15f;

const double MISSILE_SPEED = 6.0f;

const double PRECISION = 0.01f;

using namespace std;

// TODO (Samu#1#): Add some missiles and ennemies

struct BoxLogic {
    SDL_Texture * sprite;

    int directions [4];
    int orientation;

    double xPosition, yPosition;
    double xVelocity, yVelocity;

    SDL_Rect outRect;
    SDL_Rect inRect;
};

struct MissileLogic {
    SDL_Texture * sprite;

    int orientation;

    double xPosition, yPosition;
    double xVelocity, yVelocity;

    SDL_Rect outRect;
    SDL_Rect inRect;
};

MissileLogic * makeMissile (SDL_Renderer * renderer, BoxLogic & launcher) {
    MissileLogic * newMissile = new MissileLogic;

    SDL_Surface * tmp = SDL_LoadBMP("assets/missile.bmp");
    if (!tmp) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bitmap loading fail : %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    newMissile->sprite = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);

    newMissile->orientation = launcher.orientation;
    newMissile->xPosition = launcher.xPosition; newMissile->yPosition = launcher.yPosition;
    newMissile->xVelocity = ((newMissile->orientation == 1) - (newMissile->orientation == 3)) * MISSILE_SPEED;
    newMissile->yVelocity = ((newMissile->orientation == 2) - (newMissile->orientation == 0)) * MISSILE_SPEED;

    newMissile->outRect.w = newMissile->outRect.h = newMissile->inRect.w = newMissile->inRect.h = 16;
    newMissile->inRect.x = 0; newMissile->inRect.y = 16 * newMissile->orientation;
    newMissile->outRect.x = (int) floor(newMissile->xPosition + 0.5f);
    newMissile->outRect.y = (int) floor(newMissile->yPosition + 0.5f);

    return newMissile;
}

void updateMissile (MissileLogic& missile) {
    missile.xPosition += missile.xVelocity;
    missile.yPosition += missile.yVelocity;

    missile.outRect.x = (int) floor(missile.xPosition + 0.5f);
    missile.outRect.y = (int) floor(missile.yPosition + 0.5f);
}

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


/// RESOURCES INIT

    MissileLogic daMissile;

// Prepare logic
    BoxLogic daBox;

// Sprite loading
    SDL_Surface * tmp = SDL_LoadBMP("assets/daBox.bmp");
    if (!tmp) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bitmap loading fail : %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    daBox.sprite = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);

// BoxLogic fields initialisation
    for (int i = 0; i < 4; i++){
        daBox.directions[i] = 0;
    }
    daBox.orientation = 0;
    daBox.xVelocity = daBox.yVelocity = 0;

    daBox.outRect.h = daBox.outRect.w = daBox.inRect.h = daBox.inRect.w = 16;
    daBox.xPosition = (SCREEN_WIDTH-daBox.outRect.h)/2;
    daBox.yPosition = (SCREEN_HEIGHT-daBox.outRect.w)/2;
    daBox.outRect.x = (int) floor(daBox.xPosition + 0.5f);
    daBox.outRect.y = (int) floor(daBox.yPosition + 0.5f);
    daBox.inRect.x = daBox.inRect.y = 0;


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
                daMissile = *makeMissile(renderer, daBox);
                missileFired = true;
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
            }
        }

/// UPDATE
        while(lag >= SCREEN_TPF) {

        // Update the sprite according to the orientation
            daBox.inRect.y = daBox.orientation * 16;

        // Add the acceleration to the velocity
            daBox.xVelocity += (daBox.directions[1] - daBox.directions[3]) * ACC_FACTOR;
            daBox.yVelocity += (daBox.directions[2] - daBox.directions[0]) * ACC_FACTOR;

        // A portion of velocity is lost in friction
            daBox.xVelocity *= (1-FRICTION);
            daBox.yVelocity *= (1-FRICTION);

        // Check for overflows
            // Outer bounds
            if (daBox.xVelocity > MAX_SPEED) {daBox.xVelocity = MAX_SPEED;}
            if (daBox.xVelocity < -MAX_SPEED) {daBox.xVelocity = -MAX_SPEED;}
            if (daBox.yVelocity > MAX_SPEED) {daBox.yVelocity = MAX_SPEED;}
            if (daBox.yVelocity < -MAX_SPEED) {daBox.yVelocity = -MAX_SPEED;}

            // Inner bounds, so that it doesn't keep endlessly precise floats
            if (daBox.xVelocity != 0 && daBox.xVelocity < PRECISION && daBox.xVelocity > -PRECISION) {
                daBox.xVelocity = 0;
            }
            if (daBox.yVelocity != 0 && daBox.yVelocity < PRECISION && daBox.yVelocity > -PRECISION) {
                daBox.yVelocity = 0;
            }

        // At last apply the velocity vector to the position
            daBox.xPosition += daBox.xVelocity;
            daBox.yPosition += daBox.yVelocity;

        // Boundary check time !
            if (daBox.xPosition < 0) {
                daBox.xPosition = 0;
                daBox.xVelocity = 0;
            }
            if (daBox.xPosition > SCREEN_WIDTH-16) {
                daBox.xPosition = SCREEN_WIDTH-16;
                daBox.xVelocity = 0;
            }
            if (daBox.yPosition < 0) {
                daBox.yPosition = 0;
                daBox.yVelocity = 0;
            }
            if (daBox.yPosition > SCREEN_HEIGHT-16) {
                daBox.yPosition = SCREEN_HEIGHT-16;
                daBox.yVelocity = 0;
            }

        // Convert the precise position into pixels position
            daBox.outRect.x = (int) floor(daBox.xPosition + 0.5f);
            daBox.outRect.y = (int) floor(daBox.yPosition + 0.5f);

            if (missileFired)
                updateMissile(daMissile);

        // ..Aaaaand update the lag counter
            lag -= SCREEN_TPF;
        }

/// RENDER

        //cout << daBox.xVelocity << "; " << daBox.yVelocity << endl;
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, daMissile.sprite, &daMissile.inRect, &daMissile.outRect);
        SDL_RenderCopy(renderer, daBox.sprite, &daBox.inRect, &daBox.outRect);
        SDL_UpdateWindowSurface(window);

        //SDL_Delay(250);
    }

    SDL_DestroyTexture(daBox.sprite);
    daBox.sprite = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    return 0;
}
