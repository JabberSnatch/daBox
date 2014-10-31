#include "ItemLogic.h"

#include "GlobalConstants.h"

#include "stdlib.h"
#include "math.h"

void initItem(ItemLogic& item) {
    item.alive = true;

    int x = rand()%SCREEN_WIDTH - 16;
    int y = rand()%SCREEN_HEIGHT - 16;
    item.outRect.x = x;
    item.outRect.y = y;
    item.outRect.w = item.outRect.h = 16;

    item.hitBox.x = x+2;
    item.hitBox.y = y+2;
    item.hitBox.w = item.hitBox.h = 12;
}

void loadItemSprite(ItemLogic& item, SDL_Renderer* renderer) {
    SDL_Surface * tmp = SDL_LoadBMP("assets/bonus.bmp");
    if (!tmp) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bitmap loading fail : %s\n", SDL_GetError());
        return;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    item.sprite = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
}

bool collide(ItemLogic& item, BoxLogic& daBox) {
    bool collide = false;

    if (SDL_HasIntersection(&item.hitBox, &daBox.hitBox)) {
        collide = true;
        item.alive = false;
    }

    return collide;
}

void renderItem(SDL_Renderer* renderer, ItemLogic& item) {
    SDL_RenderCopy(renderer, item.sprite, NULL, &item.outRect);
}
