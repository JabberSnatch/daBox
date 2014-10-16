#include "EnemyLogic.h"

#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "GlobalConstants.h"

void initEnemy (EnemyLogic& enemy, SDL_Texture* sprite, double x, double y) {
    enemy.sprite = sprite;

    enemy.alive = true;

    enemy.xVelocity = enemy.yVelocity = 0;

    enemy.outRect.h = enemy.outRect.w = enemy.inRect.h = enemy.inRect.w = 16;
    enemy.xPosition = x;
    enemy.yPosition = y;
    enemy.outRect.x = (int) floor(enemy.xPosition + 0.5f);
    enemy.outRect.y = (int) floor(enemy.yPosition + 0.5f);
    enemy.inRect.x = enemy.inRect.y = 0;

    enemy.hitBox.h = enemy.outRect.h-2;
    enemy.hitBox.w = enemy.outRect.w-2;
    enemy.hitBox.x = enemy.outRect.x+1;
    enemy.hitBox.y = enemy.outRect.y+1;

    enemy.maxVelocity = ENEMY_MAX_SPEED;
}

void updateEnemy(EnemyLogic& enemy, BoxLogic& target) {

    double a = target.xPosition - enemy.xPosition;
    double b = target.yPosition - enemy.yPosition;
    double c = sqrt(pow(a, 2.0f)+pow(b, 2.0f)); // Distance from target
    double coeff = enemy.maxVelocity / c;

    enemy.xVelocity = a * coeff;
    enemy.yVelocity = b * coeff;

    enemy.xPosition += enemy.xVelocity;
    enemy.yPosition += enemy.yVelocity;

// Convert the precise position into pixels position
    updateOutRect(enemy);

}

void renderEnemy (SDL_Renderer* renderer, EnemyLogic& enemy) {
    SDL_RenderCopy(renderer, enemy.sprite, &enemy.inRect, &enemy.outRect);
}

EnemyLogic spawnEnemy(SDL_Texture* enemySprite) {
    EnemyLogic newEnemy;

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

    initEnemy(newEnemy, enemySprite, randX, randY);

    return newEnemy;
}

// Should collide update the entities or should it simply return a boolean ?
// Let's try updating entities
bool collide (EnemyLogic& enemy, MissileLogic& missile) {
    bool collide = false;

    if (SDL_HasIntersection(&enemy.hitBox, &missile.hitBox)) {
        collide = true;
        enemy.alive = false;
        missile.alive = false;
        std::cout << "HIT !" << std::endl;
    }

    return collide;
}

bool collide (EnemyLogic& A, EnemyLogic& B) {
    bool collide = false;

    if (SDL_HasIntersection(&A.hitBox, &B.hitBox)) {
        collide = true;
        if (A.xPosition > B.xPosition) {
            A.xPosition += ENEMY_MAX_SPEED/2;
        }
        else if (A.xPosition < B.xPosition){
            A.xPosition -= ENEMY_MAX_SPEED/2;
        }

        if (A.yPosition > B.yPosition) {
            A.yPosition += ENEMY_MAX_SPEED/2;
        }
        else if (A.yPosition < B.yPosition) {
            A.yPosition -= ENEMY_MAX_SPEED/2;
        }
    }

    updateOutRect(A);
    updateOutRect(B);

    return collide;
}

void updateOutRect(EnemyLogic& enemy){
    enemy.outRect.x = (int) floor(enemy.xPosition + 0.5f);
    enemy.outRect.y = (int) floor(enemy.yPosition + 0.5f);
    enemy.hitBox.x = enemy.outRect.x+1;
    enemy.hitBox.y = enemy.outRect.y+1;
}
