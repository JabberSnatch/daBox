#ifndef BOXLOGIC_H_INCLUDED
#define BOXLOGIC_H_INCLUDED

#include "SDL.h"

#include "MissileLogic.h"

#include <list>

struct EnemyLogic;

struct BoxLogic {
    SDL_Texture * sprite;

    bool alive;

    int directions [4];
    int orientation;

    unsigned int lastShootDate;
    bool firing;
    std::list<int> firingDirections;

    double xPosition, yPosition;
    double xVelocity, yVelocity;
    double maxVelocity;

    SDL_Rect outRect;
    SDL_Rect inRect;

    SDL_Rect hitBox;
};

void initBox (BoxLogic & daBox, SDL_Texture* sprite);
void updateBox (BoxLogic & daBox);
void renderBox (SDL_Renderer* renderer, BoxLogic & daBox);

MissileLogic fireMissile (BoxLogic& launcher, SDL_Texture* missileSprite);

bool collide(BoxLogic& daBox, EnemyLogic& enemy);

#endif // BOXLOGIC_H_INCLUDED
