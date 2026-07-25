#ifndef ENEMIES_H
#define ENEMIES_H

#include <raylib.h>
#include "game_state.h"

typedef struct Enemy{
    int health;
    int damage;
    float accuracy;
}Enemy;

typedef struct Bat{
    Enemy enemy;
    Texture *texture;
    Animation animation;
    int score_drop;
}Bat;

typedef struct Rat{
    Enemy enemy;
    Texture *texture;
    Animation animation;
    int score_drop;
}Rat;


#endif // ENEMIES_H