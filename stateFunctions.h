#ifndef STATEFUNCTIONS_H_INCLUDED
#define STATEFUNCTIONS_H_INCLUDED

// The return of each stateFunction is the id of the next state

struct Game;

/// TitleState.cpp
int titleState(Game& game); // Title screen

/// CoreState.cpp
int pauseState(Game& game);
int coreState(Game& game); // Main state of the game, where the fighting occurs

/// GameOverState.cpp
int daBoxDeath(Game& game);
int gameOverState(Game& game); // Game Over screen

#endif // STATEFUNCTIONS_H_INCLUDED
