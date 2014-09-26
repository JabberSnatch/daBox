#include "MissileLogic.h"

#include <math.h>

#include "GlobalConstants.h"

MissileLogic* makeMissile (BoxLogic& launcher, SDL_Texture* sprite) {
    MissileLogic* newMissile = new MissileLogic;

    newMissile->sprite = sprite;

    newMissile->alive = true;

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

void updateMissile (MissileLogic* missile) {
    missile->xPosition += missile->xVelocity;
    missile->yPosition += missile->yVelocity;

    if ((missile->xPosition > SCREEN_WIDTH+missile->outRect.w) || (missile->xPosition < -missile->outRect.w) ||
         (missile->yPosition > SCREEN_HEIGHT+missile->outRect.h) || (missile->yPosition < -missile->outRect.h))
         missile->alive = false;

    missile->outRect.x = (int) floor(missile->xPosition + 0.5f);
    missile->outRect.y = (int) floor(missile->yPosition + 0.5f);
}

void renderMissile (SDL_Renderer* renderer, MissileLogic* missile) {
    SDL_RenderCopy(renderer, missile->sprite, &missile->inRect, &missile->outRect);
}
