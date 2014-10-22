#include "stateFunctions.h"
#include "GlobalConstants.h"
#include "GameVariables.h"

int coreState() {
    int nextState = CORE_STATE;

/// HANDLE INPUTS

    SDL_PollEvent(&e);
    //cout << "Polling event.." << endl;

    if (e.type == SDL_QUIT) {
        running = false;
    }
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {

        case SDLK_z:
            daBox.directions[0] = 1;
            break;
        case SDLK_d:
            daBox.directions[1] = 1;
            break;
        case SDLK_s:
            daBox.directions[2] = 1;
            break;
        case SDLK_q:
            daBox.directions[3] = 1;
            break;

        case SDLK_UP:
            //cout << "UPd; ";
            daBox.orientation = 0;
            break;
        case SDLK_RIGHT:
            //cout << "RIGHTd; ";
            daBox.orientation = 1;
            break;
        case SDLK_DOWN:
            //cout << "DOWNd; ";
            daBox.orientation = 2;
            break;
        case SDLK_LEFT:
            //cout << "LEFTd; ";
            daBox.orientation = 3;
            break;

        case SDLK_SPACE:
            firing = true;
            break;

        case SDLK_ESCAPE:
            running = false;
            break;
        }
    }
    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
        case SDLK_z:
            //cout << "UPu; ";
            daBox.directions[0] = 0;
            break;
        case SDLK_d:
            //cout << "RIGHTu; ";
            daBox.directions[1] = 0;
            break;
        case SDLK_s:
            //cout << "DOWNu; ";
            daBox.directions[2] = 0;
            break;
        case SDLK_q:
            //cout << "LEFTu; ";
            daBox.directions[3] = 0;
            break;

        case SDLK_SPACE:
            firing = false;
            break;
        }
    }

/// UPDATE
    if (firing && lastShootDate + FIRING_DELAY < SDL_GetTicks()) {
        daMissiles.push_back(fireMissile(daBox, missileSprite));
        lastShootDate = SDL_GetTicks();
    }

    if (lastSpawnDate + SPAWNING_DELAY < SDL_GetTicks()) {
        spawnPack(enemySprite, daEnemies);
        lastSpawnDate = SDL_GetTicks();
    }

    while(lag >= SCREEN_TPF) {

        updateBox(daBox);

        /// Blasts update
        for (unsigned int i = 0; i < daBlasts.size(); i++) {
            if (daBlasts[i].alive) {
                updateBlast(daBlasts[i]);
            } else {
                daBlasts.erase(daBlasts.begin()+i);
            }

        }

        /// Missiles update
        for (unsigned int i = 0; i < daMissiles.size(); i++) {

            if (daMissiles[i].alive) {
                updateMissile(daMissiles[i]);;
                unsigned int j = 0;
                while(j < daEnemies.size() && daMissiles[i].alive) {
                    if (collide(daEnemies[j], daMissiles[i])) {
                        BlastLogic blast;
                        initBlast(blast, blastSprite, daEnemies[j].xPosition, daEnemies[j].yPosition);
                        daBlasts.push_back(blast);
                    }
                    j++;
                }
            }

            else {
                daMissiles.erase(daMissiles.begin()+i);
            }
        }

        /// Enemies update
        for (unsigned int i = 0; i < daEnemies.size(); i++) {

            if (daEnemies[i].alive) {
                updateEnemy(daEnemies[i], daBox);
                for (unsigned int j = i+1; j < daEnemies.size(); j++) {
                    if (i != j) {
                        collide(daEnemies[i], daEnemies[j]);
                    }
                }
                collide(daBox, daEnemies[i]);
            }

            else {
                daEnemies.erase(daEnemies.begin()+i);
            }
        }


        // ..Aaaaand update the lag counter
        lag -= SCREEN_TPF;
    }

    if (!daBox.alive) {
        //Switch to gameover state
    }

/// RENDER

    //cout << daBox.xVelocity << "; " << daBox.yVelocity << endl;
    SDL_RenderClear(renderer);

    for (unsigned int i = 0; i < daMissiles.size(); i++)
        renderMissile(renderer, daMissiles[i]);
    for (unsigned int i = 0; i < daEnemies.size(); i++)
        renderEnemy(renderer, daEnemies[i]);
    for (unsigned int i = 0; i < daBlasts.size(); i++)
        renderBlast(renderer, daBlasts[i]);
    renderBox(renderer, daBox);

    SDL_UpdateWindowSurface(window);

    return nextState;
}
