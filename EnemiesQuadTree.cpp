#include "EnemiesQuadTree.h"

#include "GlobalConstants.h"

#include <iostream>

EnemiesQuadTree::EnemiesQuadTree()
{
    bounds.x = bounds.y = 0;
    bounds.h = SCREEN_HEIGHT;
    bounds.w = SCREEN_WIDTH;
    isLeaf = true;
    depth = 0;
    for (int i = 0; i < 4; i++) {
        underQuadrants[i] = NULL;
    }
}


EnemiesQuadTree::EnemiesQuadTree(std::vector<EnemyLogic*> enemies, const SDL_Rect& bounds, int depth)
    :enemies(enemies), bounds(bounds), isLeaf(true), depth(depth)
{
    for (int i = 0; i < 4; i++) {
        underQuadrants[i] = NULL;
    }

    if (enemies.size() > MAX_ENEMIES) {
        split();
    }
}

EnemiesQuadTree::~EnemiesQuadTree() {

}


void EnemiesQuadTree::loadEnemies(std::vector<EnemyLogic>& e) {
    clear();

    for (size_t i = 0; i < e.size(); i++) {
        if (e[i].xPosition >= 0 && e[i].xPosition < SCREEN_WIDTH && e[i].yPosition >= 0 && e[i].yPosition < SCREEN_HEIGHT)
            enemies.push_back(&e[i]);
    }

    SDL_Rect newBounds;
    newBounds.x = newBounds.y = 0;
    newBounds.h = SCREEN_HEIGHT;
    newBounds.w = SCREEN_WIDTH;

    for (int i = 0; i < 4; i++) {
        underQuadrants[i] = NULL;
    }
    isLeaf = true;

    if (enemies.size() > MAX_ENEMIES) {
        split();
    }
}

// Creates the four underlying quadtrees and fills them with the appropriate entities
// Also empties the enemies list of the current quadrant, since it's not needed anymore
void EnemiesQuadTree::split() {
    SDL_Rect newBounds[4];
    for (int i = 0; i < 2; i++) { // The nested loops make it easier to process new x and y
        for (int j = 0; j < 2; j++) {
            newBounds[i*2+j].x = bounds.x+i*(bounds.w/2); // new x
            newBounds[i*2+j].y = bounds.y+j*(bounds.h/2); // new y
            newBounds[i*2+j].w = bounds.w/2;
            newBounds[i*2+j].h = bounds.h/2;
        }
    }

    std::vector<EnemyLogic*> newTabs[4];
    while(!enemies.empty()) {
        int quadrant = getQuadrant(*enemies.back());

        if (quadrant > -1) {
            newTabs[quadrant].push_back(enemies.back());
        }
        enemies.pop_back();
    }

    for (int i = 0; i < 4; i++) {
        underQuadrants[i] = new EnemiesQuadTree(newTabs[i], newBounds[i], depth+1);
    }

    isLeaf = false;
}

int EnemiesQuadTree::getQuadrant(const EnemyLogic& e) {
    int quadrant = -1;

    if (e.xPosition >= bounds.x && e.xPosition < bounds.x+bounds.w && e.yPosition >= bounds.y && e.yPosition < bounds.y+bounds.h) {
        int onWestHalf = (e.xPosition >= bounds.x+(bounds.w/2));
        int onSouthHalf = (e.yPosition >= bounds.y+(bounds.h/2));
        quadrant = (2*onWestHalf) + onSouthHalf;
    }

    return quadrant;
}

void EnemiesQuadTree::handleCollisions() {
    if (isLeaf) {
        for (unsigned int i = 0; i < enemies.size(); i++) {
            for (unsigned int j = 0; j < enemies.size(); j++) {
                collide(*enemies[i], *enemies[j]);
            }
        }
    }
    else {
        for (int i = 0; i < 4; i++) {
            underQuadrants[i]->handleCollisions();
        }
    }
}

void EnemiesQuadTree::clear() {
    if (isLeaf == false) {
        for (int i = 0; i < 4; i++) {
            underQuadrants[i]->clear();
            delete underQuadrants[i];
        }
    }
}

void EnemiesQuadTree::insert(EnemyLogic& e) {
    int quadrant = getQuadrant(e);
    std::cout << quadrant << std::endl;
    std::cout << &underQuadrants[quadrant] << std::endl;
    if (underQuadrants[quadrant] != NULL && quadrant >= 0) {
        underQuadrants[quadrant]->insert(e);
    }
    else {
        enemies.push_back(&e);
        if (enemies.size() > MAX_ENEMIES) {
            split();
        }
    }
}
