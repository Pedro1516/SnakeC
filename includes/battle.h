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

void draw_info_enemy(const Game *game, const Enemy *enemy, Texture *texture_bar);
Battle *init_battle(CombatStats *player, CombatStats *enemy, const char *texture_player_path, const char *texture_enemy_path);
void draw_entidy(Battle *battleInfo);
void draw_menu_player(const CombatStats *player, const Game *game, float timer, int *index_menu, Rectangle *rect_options);
#endif