#include "BoxLogic.h"

#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "GlobalConstants.h"

void initBox (BoxLogic & daBox, SDL_Texture * sprite) {
    initBox(daBox, sprite, (SCREEN_WIDTH-16)/2, (SCREEN_HEIGHT-16)/2);
    daBox.maxVelocity = MAX_SPEED;
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
    daBox.hitBox.x = daBox.outRect.x+1;
    daBox.hitBox.y = daBox.outRect.y+1;

    daBox.maxVelocity = ENEMY_MAX_SPEED;

    //std::cout << daBox.hitBox.h << "; " << daBox.hitBox.w << "; " << daBox.hitBox.x << "; " << daBox.hitBox.y << std::endl;
    //std::cout << daBox.outRect.h << "; " << daBox.outRect.w << "; " << daBox.outRect.x << "; " << daBox.outRect.y << std::endl;
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
    if (daBox.xVelocity > daBox.maxVelocity) {
        daBox.xVelocity = daBox.maxVelocity;
    }
    if (daBox.xVelocity < -daBox.maxVelocity) {
        daBox.xVelocity = -daBox.maxVelocity;
    }
    if (daBox.yVelocity > daBox.maxVelocity) {
        daBox.yVelocity = daBox.maxVelocity;
    }
    if (daBox.yVelocity < -daBox.maxVelocity) {
        daBox.yVelocity = -daBox.maxVelocity;
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
    daBox.hitBox.x = daBox.outRect.x+1;
    daBox.hitBox.y = daBox.outRect.y+1;
}

void updateEnemy(BoxLogic& enemy, BoxLogic& target) {
    // TODO(Samu#1#): Use pythagora and thales to find the delta X and Y of the enemy

    double a = target.xPosition - enemy.xPosition;
    double b = target.yPosition - enemy.yPosition;
    double c = sqrt(pow(a, 2.0f)+pow(b, 2.0f)); // Distance from target
    double coeff = enemy.maxVelocity / c;

    enemy.xVelocity = a * coeff;
    enemy.yVelocity = b * coeff;

    // At last apply the velocity vector to the position
    enemy.xPosition += enemy.xVelocity;
    enemy.yPosition += enemy.yVelocity;

// Convert the precise position into pixels position
    enemy.outRect.x = (int) floor(enemy.xPosition + 0.5f);
    enemy.outRect.y = (int) floor(enemy.yPosition + 0.5f);
    enemy.hitBox.x = enemy.outRect.x+1;
    enemy.hitBox.y = enemy.outRect.y+1;

}

void setDirectionsTowards(BoxLogic& daBox, BoxLogic& target) {
    if (daBox.xPosition < target.xPosition) {
        daBox.directions[1] = 1;
        daBox.directions[3] = 0;
    }
    else if (daBox.xPosition > target.xPosition) {
        daBox.directions[3] = 1;
        daBox.directions[1] = 0;
    }
    else {
        daBox.directions[1] = 0;
        daBox.directions[3] = 0;
    }

    if (daBox.yPosition < target.yPosition) {
        daBox.directions[2] = 1;
        daBox.directions[0] = 0;
    }
    else if (daBox.yPosition > target.yPosition) {
        daBox.directions[0] = 1;
        daBox.directions[2] = 0;
    }
    else {
        daBox.directions[0] = 0;
        daBox.directions[2] = 0;
    }
}

void renderBox (SDL_Renderer* renderer, BoxLogic & daBox) {
    SDL_RenderCopy(renderer, daBox.sprite, &daBox.inRect, &daBox.outRect);
}

MissileLogic fireMissile (BoxLogic& launcher, SDL_Texture* missileSprite) {
    MissileLogic newMissile;

    initMissile(newMissile, missileSprite, launcher.orientation, launcher.xPosition, launcher.yPosition);

    return newMissile;
}

BoxLogic spawnEnemy(SDL_Texture* enemySprite) {
    BoxLogic newEnemy;

    int direction = rand() % 4;
    double randX = rand() % SCREEN_WIDTH;
    double randY = rand() % SCREEN_HEIGHT;
    switch (direction) {
    case 0: // NORTH
        randY = -20;
        break;
    case 1: // EAST
        randX = SCREEN_WIDTH + 20;
        break;
    case 2: // SOUTH
        randY = SCREEN_HEIGHT + 20;
        break;
    case 3: // WEST
        randX = -20;
        break;
    }

    initBox(newEnemy, enemySprite, randX, randY);

    return newEnemy;
}

// Should collide update the entities or should it simply return a boolean ?
// Let's try updating entities
bool collide (BoxLogic& daBox, MissileLogic& missile) {
    bool collide = false;

    if (SDL_HasIntersection(&daBox.hitBox, &missile.hitBox)) {
        collide = true;
        daBox.alive = false;
        missile.alive = false;
        std::cout << "HIT !" << std::endl;
    }

    return collide;
}
