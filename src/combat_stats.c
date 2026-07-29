#include "../includes/combat_stats.h"

int combat_calculate_damage(const CombatStats *attacker, const CombatStats *target)
{
    int damage = attacker->attack - target->defense;
    return damage > 0 ? damage : 1;
}

void combat_heal(CombatStats *stats, int amount)
{
    int new_health = stats->health + amount;
    stats->health = new_health > stats->max_health ? stats->max_health : new_health;
}

void combat_take_damage(CombatStats *stats, int damage)
{
    int new_health = stats->health - damage;
    stats->health = new_health < 0 ? 0 : new_health;
}

bool combat_is_dead(const CombatStats *stats)
{
    return (bool)(stats->health <= 0);
}