#include "BoxLogic.h"

#include <math.h>
#include <iostream>

#include "GlobalConstants.h"

void initBox (BoxLogic & daBox, SDL_Texture * sprite) {
    initBox(daBox, sprite, (SCREEN_WIDTH-16)/2, (SCREEN_HEIGHT-16)/2);
}

void initBox (BoxLogic& daBox, SDL_Texture* sprite, double x, double y) {
    daBox.sprite = sprite;

    daBox.alive = true;

    for (int i = 0; i < 4; i++){
        daBox.directions[i] = 0;
    }
    daBox.orientation = 0;
    daBox.xVelocity = daBox.yVelocity = 0;

    daBox.outRect.h = daBox.outRect.w = daBox.inRect.h = daBox.inRect.w = 16;
    daBox.xPosition = x;
    daBox.yPosition = y;
    daBox.outRect.x = (int) floor(daBox.xPosition + 0.5f);
    daBox.outRect.y = (int) floor(daBox.yPosition + 0.5f);
    daBox.inRect.x = daBox.inRect.y = 0;

    daBox.hitBox.h = daBox.outRect.h-2;
    daBox.hitBox.w = daBox.outRect.w-2;
    daBox.hitBox.x = daBox.outRect.x;
    daBox.hitBox.y = daBox.outRect.y;

    std::cout << daBox.hitBox.h << "; " << daBox.hitBox.w << ";" << daBox.hitBox.x << "; " << daBox.hitBox.y << std::endl;
    std::cout << daBox.outRect.h << "; " << daBox.outRect.w << ";" << daBox.outRect.x << "; " << daBox.outRect.y << std::endl;
}

void updateBox (BoxLogic & daBox) {
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
    if (daBox.xVelocity > MAX_SPEED) {
        daBox.xVelocity = MAX_SPEED;
    }
    if (daBox.xVelocity < -MAX_SPEED) {
        daBox.xVelocity = -MAX_SPEED;
    }
    if (daBox.yVelocity > MAX_SPEED) {
        daBox.yVelocity = MAX_SPEED;
    }
    if (daBox.yVelocity < -MAX_SPEED) {
        daBox.yVelocity = -MAX_SPEED;
    }

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
    daBox.hitBox.x = daBox.outRect.x;
    daBox.hitBox.y = daBox.outRect.y;
}

void renderBox (SDL_Renderer* renderer, BoxLogic & daBox) {
    SDL_RenderCopy(renderer, daBox.sprite, &daBox.inRect, &daBox.outRect);
}

MissileLogic* fireMissile (BoxLogic& launcher, SDL_Texture* missileSprite) {
    MissileLogic* newMissile = new MissileLogic;

    initMissile(newMissile, missileSprite, launcher.orientation, launcher.xPosition, launcher.yPosition);

    return newMissile;
}

// Should collide update the entities or should it simply return a boolean ?
// Let's try updating entities
bool collide (BoxLogic* daBox, MissileLogic* missile) {
    bool collide = false;

    if (SDL_HasIntersection(&daBox->hitBox, &missile->hitBox)) {
        collide = true;
        //daBox->alive = false;
        missile->alive = false;
        std::cout << missile->hitBox.w << "; " << missile->hitBox.h << "; " << missile->hitBox.x << "; " << missile->hitBox.y << std::endl;
        std::cout << missile->outRect.w << "; " << missile->outRect.h << "; " << missile->outRect.x << "; " << missile->outRect.y << std::endl;
        std::cout << "HIT !" << std::endl;
    }

    return collide;
}
