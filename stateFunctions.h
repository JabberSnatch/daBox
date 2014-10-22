#ifndef STATEFUNCTIONS_H_INCLUDED
#define STATEFUNCTIONS_H_INCLUDED

// The return of each stateFunction is the id of the next state

struct Game;

int titleState(Game& game); // Title screen
int coreState(Game& game); // Main state of the game, where the fighting occurs
int gameOverState(Game& game); // Game Over screen

#endif // STATEFUNCTIONS_H_INCLUDED
