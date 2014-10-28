#include "StateFunctions.h"
#include "GlobalConstants.h"
#include "GameVariables.h"

#include <iostream>

int pauseState(Game& game) {
    int nextState = PAUSE_STATE;

    if (game.e.type == SDL_QUIT) {
        game.running = false;
    }
    if (game.e.type == SDL_KEYDOWN) {
        switch(game.e.key.keysym.sym) {
        case SDLK_ESCAPE:
            game.running = false;
            break;
        case SDLK_p:
            nextState = CORE_STATE;
            game.lastTime = SDL_GetTicks();
            game.lag = 0;
            break;
        }
    }

    return nextState;
}

int coreState(Game& game) {
    int nextState = CORE_STATE;

    game.updated = false;

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
            game.daBox.firing = true;
            break;

        case SDLK_ESCAPE:
            game.running = false;
            break;

        case SDLK_p:
            nextState = PAUSE_STATE;
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
            game.daBox.firing = false;
            break;
        }
    }

/// UPDATE

    // FIRING MISSILE
    if (game.daBox.firing && game.daBox.lastShootDate + FIRING_DELAY < SDL_GetTicks()) {
        game.daMissiles.push_back(fireMissile(game.daBox, game.missileSprite));
        game.daBox.lastShootDate = SDL_GetTicks();
    }

    // SPAWNING PACKS OF ENEMIES
    if (game.lastSpawnDate + SPAWNING_DELAY < SDL_GetTicks() && game.daEnemies.size() < ENEMIES_CAP) {
        spawnPack(game.enemySprite, game.daEnemies);
        game.lastSpawnDate = SDL_GetTicks();
    }

    // GAME LOOP
    while(game.lag >= SCREEN_TPF) {

        /// Box update
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
        unsigned int j;
        BlastLogic blast;
        for (unsigned int i = 0; i < game.daMissiles.size(); i++) {

            if (game.daMissiles[i].alive) {

                updateMissile(game.daMissiles[i]); // Move the missile

                // Look for collisions with the enemies
                j = 0;
                while(j < game.daEnemies.size() && game.daMissiles[i].alive) {
                    if (collide(game.daEnemies[j], game.daMissiles[i])) {
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
                for (unsigned int j = 0; j < game.daEnemies.size(); j++) {
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
        game.updated = true;
    }

/// RENDER

    //cout << daBox.xVelocity << "; " << daBox.yVelocity << endl;
    renderAll(game);

    if (game.updated){
        int ticks = SDL_GetTicks();
        std::cout << game.daEnemies.size() << " : " << 1000/(ticks - game.lastRender + 1) << std::endl; //Add 1 to prevent division by 0
        game.lastRender = ticks;
    }


    if (!game.daBox.alive) {
        game.lives.erase(game.lives.end()-1);
        game.daBox.alive = true;
        nextState = DABOXDEATH_STATE;
    }

    return nextState;
}
