#ifndef GAMEVARIABLES_H_INCLUDED
#define GAMEVARIABLES_H_INCLUDED

#include "SDL.h"

#include "MissileLogic.h"
#include "EnemyLogic.h"
#include "BlastLogic.h"
#include "BoxLogic.h"
#include "ItemLogic.h"

#include <vector>

struct Game {
/// SDL
    SDL_Window *window;
    SDL_Surface *screen;
    SDL_Renderer *renderer;

/// Sprites
    SDL_Texture* boxSprite;
    SDL_Texture* missileSprite;
    SDL_Texture* enemySprite;
    SDL_Texture* blastSprite;
    SDL_Texture* heartSprite;
    SDL_Texture* gameoverScreen;

/// Entities
    std::vector<MissileLogic> daMissiles;
    std::vector<EnemyLogic> daEnemies;
    std::vector<BlastLogic> daBlasts;
    std::vector<BlastLogic> lives;
    ItemLogic bonusItem;
    BoxLogic daBox;

    unsigned int lastSpawnDate;

    unsigned int score;
    double multiplier;

/// Core
    bool running;
    SDL_Event e;
    unsigned int lastTime, currentTime, lag, elapsed; // FPS capping variables

    unsigned int lastRender; // FPS displaying variables
    bool updated;

    int state;
};

void resetGame(Game& game);
void renderAll(Game& game); //TODO implement this function using the render code in CoreState

void renderScore(Game& game);

#endif // GAMEVARIABLES_H_INCLUDED
