#ifndef ITEMLOGIC_H_INCLUDED
#define ITEMLOGIC_H_INCLUDED

#include "SDL.h"

#include "BoxLogic.h"

struct ItemLogic {
    SDL_Texture * sprite;

    bool alive;

    SDL_Rect outRect;
    SDL_Rect hitBox;
};

void initItem(ItemLogic& item);
void loadItemSprite(ItemLogic& item, SDL_Renderer* renderer);
bool collide(ItemLogic& item, BoxLogic& daBox);
void renderItem(SDL_Renderer* renderer, ItemLogic& item);

#endif // ITEMLOGIC_H_INCLUDED
