#include "MissileLogic.h"

#include <math.h>

#include "GlobalConstants.h"

void initMissile (MissileLogic* missile, SDL_Texture* sprite, int orientation, double x, double y) {
    missile->sprite = sprite;

    missile->alive = true;

    missile->orientation = orientation;
    missile->xPosition = x;
    missile->yPosition = y;
    missile->xVelocity = ((missile->orientation == 1) - (missile->orientation == 3)) * MISSILE_SPEED;
    missile->yVelocity = ((missile->orientation == 2) - (missile->orientation == 0)) * MISSILE_SPEED;

    missile->outRect.w = missile->outRect.h = missile->inRect.w = missile->inRect.h = 16;
    missile->inRect.x = 0;
    missile->inRect.y = 16 * missile->orientation;
    missile->outRect.x = (int) floor(missile->xPosition + 0.5f);
    missile->outRect.y = (int) floor(missile->yPosition + 0.5f);

    missile->hitBox.w = ((missile->orientation == 1 || missile->orientation == 3) * 14) +
                            ((missile->orientation == 2 || missile->orientation == 0) * 2);
    missile->hitBox.h = ((missile->hitBox.w == 14) * 2) + ((missile->hitBox.w == 2) * 14);
    missile->hitBox.x = missile->outRect.x;
    missile->hitBox.y = missile->outRect.y;
}

void updateMissile (MissileLogic* missile) {
    missile->xPosition += missile->xVelocity;
    missile->yPosition += missile->yVelocity;

    if ((missile->xPosition > SCREEN_WIDTH+missile->outRect.w) || (missile->xPosition < -missile->outRect.w) ||
         (missile->yPosition > SCREEN_HEIGHT+missile->outRect.h) || (missile->yPosition < -missile->outRect.h))
         missile->alive = false;

    missile->outRect.x = (int) floor(missile->xPosition + 0.5f);
    missile->outRect.y = (int) floor(missile->yPosition + 0.5f);
    missile->hitBox.x = missile->outRect.x;
    missile->hitBox.y = missile->outRect.y;
}

void renderMissile (SDL_Renderer* renderer, MissileLogic* missile) {
    SDL_RenderCopy(renderer, missile->sprite, &missile->inRect, &missile->outRect);
}
