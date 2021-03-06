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

    enemy.speed = ENEMY_MAX_SPEED;
}

void updateEnemy(EnemyLogic& enemy, BoxLogic& target) {

    double a = target.xPosition - enemy.xPosition;
    double b = target.yPosition - enemy.yPosition;
    double c = sqrt(pow(a, 2.0f)+pow(b, 2.0f)); // Distance from target
    double coeff = enemy.speed / c;

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

// Packs of enemies are spawned outside the screen.
// It chooses a random point and spawns enemy near it
void spawnPack(SDL_Texture* enemySprite, std::vector<EnemyLogic>& enemies) {

    int offset = 60; // How far away from the side the spawning point will be

    SDL_Point packCenter;
    packCenter.x = rand()%SCREEN_WIDTH;
    packCenter.y = rand()%SCREEN_HEIGHT;

    std::cout << packCenter.x << "; " << packCenter.y << std::endl;

    int side = rand() % 4;
    switch (side) {
    case 0: // NORTH
        packCenter.y = -offset;
        break;
    case 1: // EAST
        packCenter.x = SCREEN_WIDTH + offset;
        break;
    case 2: // SOUTH
        packCenter.y = SCREEN_HEIGHT + offset;
        break;
    case 3: // WEST
        packCenter.x = -offset;
        break;
    }

    int enemiesToSpawn = (rand() % 15) + 5;
    int range = enemiesToSpawn * 2 + 19;

    for (int i = 0; i < enemiesToSpawn; i++){
        double x = packCenter.x + rand()%(range*2) - range;
        double y = packCenter.y + rand()%(range*2) - range;

        EnemyLogic newEnemy;
        initEnemy(newEnemy, enemySprite, x, y);
        enemies.push_back(newEnemy);
    }
}

// Should collide update the entities or should it simply return a boolean ?
// Let's try updating entities
bool collide (EnemyLogic& enemy, MissileLogic& missile) {
    bool collide = false;

    if (SDL_HasIntersection(&enemy.hitBox, &missile.hitBox)) {
        collide = true;
        enemy.alive = false;
        missile.alive = false;
        //std::cout << "HIT !" << std::endl;
    }

    return collide;
}

bool collide (EnemyLogic& A, EnemyLogic& B) {
    bool collide = false;

    if (SDL_HasIntersection(&A.hitBox, &B.hitBox)) {
        collide = true;
        if (A.xPosition > B.xPosition) {
            A.xPosition += SHIFTING_DISTANCE;
        }
        else if (A.xPosition < B.xPosition){
            A.xPosition -= SHIFTING_DISTANCE;
        }

        if (A.yPosition > B.yPosition) {
            A.yPosition += SHIFTING_DISTANCE;
        }
        else if (A.yPosition < B.yPosition) {
            A.yPosition -= SHIFTING_DISTANCE;
        }
    }

    updateOutRect(A);
    updateOutRect(B);

    return collide;
}

void updateOutRect(EnemyLogic& enemy){
    enemy.outRect.x = (int) enemy.xPosition;
    enemy.outRect.y = (int) enemy.yPosition;
    enemy.hitBox.x = enemy.outRect.x+1;
    enemy.hitBox.y = enemy.outRect.y+1;
}
