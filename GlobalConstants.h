#ifndef GLOBALCONSTANTS_H_INCLUDED
#define GLOBALCONSTANTS_H_INCLUDED

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

const unsigned int SCREEN_FPS = 59;
const unsigned int SCREEN_TPF = 1000 / SCREEN_FPS; // TPF = Ticks Per Frame

const double ENEMY_MAX_SPEED = 1.7f;
const double MAX_SPEED = 3.7f;
const double ACC_FACTOR = 0.7f;
const double FRICTION = 0.15f;

const double MISSILE_SPEED = 6.0f;
const unsigned int FIRING_DELAY = 100; // Time between each missile, in ms

const double PRECISION = 0.01f;

#endif // GLOBALCONSTANTS_H_INCLUDED
