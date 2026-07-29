#ifndef BATTLE_H
#define BATTLE_H
#include <raylib.h>
#include "combat_stats.h"
#include "enemy.h"

typedef struct
{
    Camera3D cam;
    Vector3 center;

    float camera_angle;
    float camera_radius;
    float camera_height;
} BattleCam;

typedef struct
{
    Animation animation;
    Texture2D texture;
    Vector3 position;
} BattleEntidy;

typedef struct
{
    BattleCam camera_battle;

    BattleEntidy player;
    BattleEntidy enemy;

    CombatStats *player_data;
    CombatStats *enemy_data;
} Battle;

void draw_info_enemy(const Game *game, const Enemy *enemy);
Battle *init_battle(CombatStats *player, CombatStats *enemy);
void draw_entidy(Battle *battleInfo);

#endif