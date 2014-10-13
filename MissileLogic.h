#ifndef MISSILELOGIC_H_INCLUDED
#define MISSILELOGIC_H_INCLUDED

#include "SDL.h"

struct MissileLogic {
    SDL_Texture * sprite;

    bool alive;

    int orientation;

    double xPosition, yPosition;
    double xVelocity, yVelocity;

    SDL_Rect outRect;
    SDL_Rect inRect;

    SDL_Rect hitBox;
};

void initMissile (MissileLogic& missile, SDL_Texture* sprite, int orientation, double x, double y);
void updateMissile (MissileLogic& missile);
void renderMissile (SDL_Renderer* renderer, MissileLogic& missile);

#endif // MISSILELOGIC_H_INCLUDED
