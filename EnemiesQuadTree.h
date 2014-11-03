#ifndef ENEMIESQUADTREE_H_INCLUDED
#define ENEMIESQUADTREE_H_INCLUDED

#include "SDL.h"

#include "EnemyLogic.h"

#include <vector>

class EnemiesQuadTree {

    static const unsigned int MAX_ENEMIES = 8;

private:
    std::vector<EnemyLogic*> enemies;
    SDL_Rect bounds;
    EnemiesQuadTree* underQuadrants [4];
    bool isLeaf;
    int depth;

    void split();
    int getQuadrant(const EnemyLogic& e);

public:
    EnemiesQuadTree();
    EnemiesQuadTree(std::vector<EnemyLogic*> enemies, const SDL_Rect& bounds, int depth);
    ~EnemiesQuadTree();

    void loadEnemies(std::vector<EnemyLogic>& e);
    void handleCollisions();
    void clear();
    void insert(EnemyLogic& e);

};

#endif // ENEMIESQUADTREE_H_INCLUDED
