#include "stateFunctions.h"
#include "GlobalConstants.h"
#include "gameVariables.h"

int coreState(Game& game) {
    int nextState = CORE_STATE;

/// HANDLE INPUTS
    if (game.e.type == SDL_QUIT) {
        game.running = false;
    }
    if (game.e.type == SDL_KEYDOWN) {
        switch (game.e.key.keysym.sym) {

        case SDLK_z:
            game.daBox.directions[0] = 1;
            break;
        case SDLK_d:
            game.daBox.directions[1] = 1;
            break;
        case SDLK_s:
            game.daBox.directions[2] = 1;
            break;
        case SDLK_q:
            game.daBox.directions[3] = 1;
            break;

        case SDLK_UP:
            //cout << "UPd; ";
            game.daBox.orientation = 0;
            break;
        case SDLK_RIGHT:
            //cout << "RIGHTd; ";
            game.daBox.orientation = 1;
            break;
        case SDLK_DOWN:
            //cout << "DOWNd; ";
            game.daBox.orientation = 2;
            break;
        case SDLK_LEFT:
            //cout << "LEFTd; ";
            game.daBox.orientation = 3;
            break;

        case SDLK_SPACE:
            game.firing = true;
            break;

        case SDLK_ESCAPE:
            game.running = false;
            break;
        }
    }
    if (game.e.type == SDL_KEYUP) {
        switch (game.e.key.keysym.sym) {
        case SDLK_z:
            //cout << "UPu; ";
            game.daBox.directions[0] = 0;
            break;
        case SDLK_d:
            //cout << "RIGHTu; ";
            game.daBox.directions[1] = 0;
            break;
        case SDLK_s:
            //cout << "DOWNu; ";
            game.daBox.directions[2] = 0;
            break;
        case SDLK_q:
            //cout << "LEFTu; ";
            game.daBox.directions[3] = 0;
            break;

        case SDLK_SPACE:
            game.firing = false;
            break;
        }
    }

/// UPDATE
    if (game.firing && game.lastShootDate + FIRING_DELAY < SDL_GetTicks()) {
        game.daMissiles.push_back(fireMissile(game.daBox, game.missileSprite));
        game.lastShootDate = SDL_GetTicks();
    }

    if (game.lastSpawnDate + SPAWNING_DELAY < SDL_GetTicks()) {
        spawnPack(game.enemySprite, game.daEnemies);
        game.lastSpawnDate = SDL_GetTicks();
    }

    while(game.lag >= SCREEN_TPF) {

        updateBox(game.daBox);

        /// Blasts update
        for (unsigned int i = 0; i < game.daBlasts.size(); i++) {
            if (game.daBlasts[i].alive) {
                updateBlast(game.daBlasts[i]);
            } else {
                game.daBlasts.erase(game.daBlasts.begin()+i);
            }

        }

        /// Missiles update
        for (unsigned int i = 0; i < game.daMissiles.size(); i++) {

            if (game.daMissiles[i].alive) {
                updateMissile(game.daMissiles[i]);;
                unsigned int j = 0;
                while(j < game.daEnemies.size() && game.daMissiles[i].alive) {
                    if (collide(game.daEnemies[j], game.daMissiles[i])) {
                        BlastLogic blast;
                        initBlast(blast, game.blastSprite, game.daEnemies[j].xPosition, game.daEnemies[j].yPosition);
                        game.daBlasts.push_back(blast);
                    }
                    j++;
                }
            }

            else {
                game.daMissiles.erase(game.daMissiles.begin()+i);
            }
        }

        /// Enemies update
        for (unsigned int i = 0; i < game.daEnemies.size(); i++) {

            if (game.daEnemies[i].alive) {
                updateEnemy(game.daEnemies[i], game.daBox);
                for (unsigned int j = i+1; j < game.daEnemies.size(); j++) {
                    if (i != j) {
                        collide(game.daEnemies[i], game.daEnemies[j]);
                    }
                }
                collide(game.daBox, game.daEnemies[i]);
            }

            else {
                game.daEnemies.erase(game.daEnemies.begin()+i);
            }
        }


        // ..Aaaaand update the lag counter
        game.lag -= SCREEN_TPF;
    }

    if (!game.daBox.alive) {
        //Switch to gameover state
    }

/// RENDER

    //cout << daBox.xVelocity << "; " << daBox.yVelocity << endl;
    SDL_RenderClear(game.renderer);

    for (unsigned int i = 0; i < game.daMissiles.size(); i++)
        renderMissile(game.renderer, game.daMissiles[i]);
    for (unsigned int i = 0; i < game.daEnemies.size(); i++)
        renderEnemy(game.renderer, game.daEnemies[i]);
    for (unsigned int i = 0; i < game.daBlasts.size(); i++)
        renderBlast(game.renderer, game.daBlasts[i]);
    renderBox(game.renderer, game.daBox);

    SDL_UpdateWindowSurface(game.window);

    return nextState;
}
