#ifndef COMBAT_STATS_H
#define COMBAT_STATS_H
#include <stdbool.h>

typedef struct CombatStats
{
    int max_health;
    int health;

    int attack;
    int defense;

    int acuracy;
    int lucky;
} CombatStats;

int combat_calculate_damage(const CombatStats *attacker, const CombatStats *target);

void combat_heal(CombatStats *stats, int amount);

void combat_take_damage(CombatStats *stats, int damage);

bool combat_is_dead(const CombatStats *stats);

#endif // COMBAT_H