#ifndef STATEFUNCTIONS_H_INCLUDED
#define STATEFUNCTIONS_H_INCLUDED

// The return of each stateFunction is the id of the next state

int titleState(); // Title screen
int coreState(); // Main state of the game, where the fighting occurs
int gameOverState(); // Game Over screen

#endif // STATEFUNCTIONS_H_INCLUDED
