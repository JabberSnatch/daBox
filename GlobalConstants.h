#ifndef GLOBALCONSTANTS_H_INCLUDED
#define GLOBALCONSTANTS_H_INCLUDED

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

const unsigned int SCREEN_FPS = 59;
const unsigned int SCREEN_TPF = 1000 / SCREEN_FPS; // TPF = Ticks Per Frame

const double ENEMY_MAX_SPEED = 2.0f;
const double SHIFTING_DISTANCE = ENEMY_MAX_SPEED / 2;

const double MAX_SPEED = 3.7f;
const double ACC_FACTOR = 0.7f;
const double FRICTION = 0.15f;

const double MISSILE_SPEED = 6.0f;
const unsigned int FIRING_DELAY = 100; // Time between each missile, in ms

const unsigned int SPAWNING_DELAY = 2000;

const double PRECISION = 0.1f;

const int CORE_STATE = 0;
const int TITLE_STATE = 1;
const int GAMEOVER_STATE = 2;

const int ENEMIES_CAP = 325;

#endif // GLOBALCONSTANTS_H_INCLUDED
