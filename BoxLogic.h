#ifndef BOXLOGIC_H_INCLUDED
#define BOXLOGIC_H_INCLUDED

#include "SDL.h"

struct BoxLogic {
    SDL_Texture * sprite;

    int directions [4];
    int orientation;

    double xPosition, yPosition;
    double xVelocity, yVelocity;

    SDL_Rect outRect;
    SDL_Rect inRect;
};

void initBox (BoxLogic & daBox, SDL_Texture* sprite);
void initBox (BoxLogic & daBox, SDL_Texture* sprite, double x, double y);
void updateBox (BoxLogic & daBox);
void renderBox (SDL_Renderer* renderer, BoxLogic & daBox);

#endif // BOXLOGIC_H_INCLUDED
