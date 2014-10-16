#ifndef ENEMYLOGIC_H_INCLUDED
#define ENEMYLOGIC_H_INCLUDED

#include "SDL.h"

#include "MissileLogic.h"
#include "BoxLogic.h"

struct EnemyLogic {
    SDL_Texture* sprite;

    bool alive;

    double xPosition, yPosition;
    double xVelocity, yVelocity;
    double maxVelocity;

    SDL_Rect outRect;
    SDL_Rect inRect;

    SDL_Rect hitBox;
};


void initEnemy (EnemyLogic& enemy, SDL_Texture* sprite, double x, double y);
void updateEnemy (EnemyLogic& enemy, BoxLogic& target); // What would be really neat is an entity     superclass
void renderEnemy (SDL_Renderer* renderer, EnemyLogic& enemy);

EnemyLogic spawnEnemy(SDL_Texture* sprite);

bool collide (EnemyLogic& enemy, MissileLogic& missile);

#endif // ENEMYLOGIC_H_INCLUDED
