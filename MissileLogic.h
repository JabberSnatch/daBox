#ifndef MISSILELOGIC_H_INCLUDED
#define MISSILELOGIC_H_INCLUDED

#include "SDL.h"

#include "BoxLogic.h"

struct MissileLogic {
    SDL_Texture * sprite;

    bool alive;

    int orientation;

    double xPosition, yPosition;
    double xVelocity, yVelocity;

    SDL_Rect outRect;
    SDL_Rect inRect;
};

MissileLogic* makeMissile (BoxLogic& launcher, SDL_Texture* sprite);
void updateMissile (MissileLogic* missile);
void renderMissile (SDL_Renderer* renderer, MissileLogic* missile);

#endif // MISSILELOGIC_H_INCLUDED
