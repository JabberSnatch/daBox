#include "BlastLogic.h"
#include "math.h"

void initBlast(BlastLogic& blast, SDL_Texture* sprite, double x, double y) {
    blast.sprite = sprite;
    blast.alive = true;
    blast.currentFrame = 0;

    blast.outRect.h = blast.outRect.w = blast.inRect.h = blast.inRect.w = 16;
    blast.outRect.x = (int) floor(x + 0.5f);
    blast.outRect.y = (int) floor(y + 0.5f);

    blast.inRect.x = blast.inRect.y = 0;
}

void updateBlast(BlastLogic& blast) {
    if (blast.currentFrame < 7) {
        blast.currentFrame++;
        blast.inRect.y = blast.currentFrame * blast.inRect.h;
    }
    else {
        blast.alive = false;
    }
}

void renderBlast(SDL_Renderer* renderer, BlastLogic& blast) {
    SDL_RenderCopy(renderer, blast.sprite, &blast.inRect, &blast.outRect);
}
