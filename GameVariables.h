#ifndef GAMEVARIABLES_H_INCLUDED
#define GAMEVARIABLES_H_INCLUDED

#include "SDL.h"

#include "MissileLogic.h"
#include "EnemyLogic.h"
#include "BlastLogic.h"
#include "BoxLogic.h"

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

/// Entities
std::vector<MissileLogic> daMissiles;
std::vector<EnemyLogic> daEnemies;
std::vector<BlastLogic> daBlasts;
BoxLogic daBox;

unsigned int lastSpawnDate;

bool running;
SDL_Event e;
unsigned int lastTime, currentTime, lag, elapsed; // FPS capping variables
};

#endif // GAMEVARIABLES_H_INCLUDED
