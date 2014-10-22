#ifndef BLASTLOGIC_H_INCLUDED
#define BLASTLOGIC_H_INCLUDED

#include "SDL.h"

struct BlastLogic {
    SDL_Texture * sprite;

    bool alive;

    int currentFrame;

    SDL_Rect outRect;
    SDL_Rect inRect;
};

void initBlast(BlastLogic& blast, SDL_Texture* sprite, double x, double y);
void updateBlast(BlastLogic& blast);
void renderBlast(SDL_Renderer* renderer, BlastLogic& blast);

#endif // BLASTLOGIC_H_INCLUDED
