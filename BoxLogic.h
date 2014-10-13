#ifndef BOXLOGIC_H_INCLUDED
#define BOXLOGIC_H_INCLUDED

#include "SDL.h"

#include "MissileLogic.h"

// TODO(Samu#1#): Decoupling enemy and box might be a good idea...

struct BoxLogic {
    SDL_Texture * sprite;

    bool alive;

    int directions [4];
    int orientation;

    double xPosition, yPosition;
    double xVelocity, yVelocity;
    double maxVelocity;

    SDL_Rect outRect;
    SDL_Rect inRect;

    SDL_Rect hitBox;
};

void initBox (BoxLogic & daBox, SDL_Texture* sprite);
void initBox (BoxLogic & daBox, SDL_Texture* sprite, double x, double y);

void updateBox (BoxLogic & daBox);
void updateEnemy (BoxLogic& enemy, BoxLogic& target);

void setDirectionsTowards (BoxLogic& daBox, BoxLogic& target);

void renderBox (SDL_Renderer* renderer, BoxLogic & daBox);

MissileLogic fireMissile (BoxLogic& launcher, SDL_Texture* missileSprite);
BoxLogic spawnEnemy (SDL_Texture* enemySprite);

bool collide (BoxLogic& daBox, MissileLogic& missile);

#endif // BOXLOGIC_H_INCLUDED
