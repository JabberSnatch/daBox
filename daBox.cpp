#include <iostream>

#include "SDL.h"


const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

const unsigned int SCREEN_FPS = 60;
const unsigned int SCREEN_TPF = 1000 / SCREEN_FPS; // TPF = Ticks Per Frame

const unsigned int BOX_SPEED = 2;

using namespace std;

// TODO (Samu#2#): Acceleration FX - Weeeee!                        \ \ \ \ \
// TODO (Samu#2#): Inertia FX - Lat's add some curves to this box   / / / / /
// TODO (Samu#3#): Add some missiles and ennemies

struct BoxLogic {
    SDL_Texture * sprite;

    int directions [4];
    int orientation;

    SDL_Rect outRect;
    SDL_Rect inRect;
};

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
    daBox.directions[0]=0; daBox.directions[1]=0; daBox.directions[2]=0; daBox.directions[3]=0;
    daBox.orientation = 0;

    daBox.outRect.h = daBox.outRect.w = daBox.inRect.h = daBox.inRect.w = 16;
    daBox.outRect.x = (SCREEN_WIDTH-daBox.outRect.h)/2;
    daBox.outRect.y = (SCREEN_HEIGHT-daBox.outRect.w)/2;
    daBox.inRect.x = daBox.inRect.y = 0;


/// GAME LOOP INIT

    bool running = true;
    SDL_Event e;
    unsigned int lastTime = SDL_GetTicks(), currentTime, lag, elapsed; // FPS capping variables

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

            case SDLK_UP:
                //cout << "UPd; ";
                daBox.directions[0] = 1;
                break;
            case SDLK_RIGHT:
                //cout << "RIGHTd; ";
                daBox.directions[1] = 1;
                break;
            case SDLK_DOWN:
                //cout << "DOWNd; ";
                daBox.directions[2] = 1;
                break;
            case SDLK_LEFT:
                //cout << "LEFTd; ";
                daBox.directions[3] = 1;
                break;

            case SDLK_z:
                daBox.orientation = 0;
                break;
            case SDLK_d:
                daBox.orientation = 1;
                break;
            case SDLK_s:
                daBox.orientation = 2;
                break;
            case SDLK_q:
                daBox.orientation = 3;
                break;


            case SDLK_ESCAPE:
                running = false;
                break;
            }
        }
        if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                //cout << "UPu; ";
                daBox.directions[0] = 0;
                break;
            case SDLK_RIGHT:
                //cout << "RIGHTu; ";
                daBox.directions[1] = 0;
                break;
            case SDLK_DOWN:
                //cout << "DOWNu; ";
                daBox.directions[2] = 0;
                break;
            case SDLK_LEFT:
                //cout << "LEFTu; ";
                daBox.directions[3] = 0;
                break;
            }
        }

/// UPDATE
        while(lag >= SCREEN_TPF) {

            /*int simultaneousDir = 0;
            int tmpDir = 0;
            for (int i = 0; i < 4; i++){
                simultaneousDir += daBox.directions[i];
                if (daBox.directions[i] == 1)
                    tmpDir = i;
            }
            if (simultaneousDir == 1)
                daBox.orientation = tmpDir;*/

            daBox.inRect.y = daBox.orientation * 16;


            daBox.outRect.y += (daBox.directions[2] - daBox.directions[0])*BOX_SPEED;
            daBox.outRect.x += (daBox.directions[1] - daBox.directions[3])*BOX_SPEED;

            if (daBox.outRect.y < 0) daBox.outRect.y = 0;
            if (daBox.outRect.y > 480-16) daBox.outRect.y = 480-16;
            if (daBox.outRect.x < 0) daBox.outRect.x = 0;
            if (daBox.outRect.x > 640-16) daBox.outRect.x = 640-16;


            lag -= SCREEN_TPF;
        }

/// RENDER

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, daBox.sprite, &daBox.inRect, &daBox.outRect);
        SDL_UpdateWindowSurface(window);
    }

    //SDL_FreeSurface(daBox.sprite);
    daBox.sprite = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    return 0;
}
