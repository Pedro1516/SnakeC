#ifndef ENEMIES_H
#define ENEMIES_H

#include <raylib.h>
#include "game_state.h"
#include "combat_stats.h"
#include "types.h"

typedef struct Enemy
{
    CombatStats status;
    const char *name;
    int level;
} Enemy;

typedef struct Bat
{
    Enemy enemy;
    Texture *texture;
    Animation animation;
    int score_drop;
} Bat;

typedef struct Rat
{
    Enemy enemy;
    Texture *texture;
    Animation animation;
    int score_drop;
} Rat;

#endif // ENEMIES_H