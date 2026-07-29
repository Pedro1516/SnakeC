#include "../includes/game_state.h"
#include "../includes/enemy.h"
#include "../includes/battle.h"
#include "../includes/game_state.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

void draw_health_bar(const Enemy *enemy, int posX, int posY, int width, int height)
{
    Rectangle rec_background = {posX, posY, width, height};
    Rectangle rec_heath = rec_background;
    rec_heath.width = enemy->status.health >= 0 ? (width * enemy->status.health) / enemy->status.max_health : 0;

    DrawRectangleRoundedLinesEx(rec_background, 0.8f, 30, 2, BLACK);
    DrawRectangleRounded(rec_background, 0.8f, 10, GRAY); // Background
    DrawRectangleRounded(rec_heath, 0.8f, 10, RED);       // Health
}

void draw_info_enemy(const Game *game, const Enemy *enemy)
{
    Rectangle rec_menu = {
        10,
        10,
        game->screenWidth / 2,
        100,
    };

    DrawRectangleRounded(rec_menu, 0.4f, 30, RAYWHITE);
    DrawRectangleRoundedLinesEx((Rectangle){rec_menu.x, rec_menu.y, rec_menu.width, rec_menu.height}, 0.4f, 30, 2, BLACK);

    int font_size = 20;
    const char *text_name = enemy->name;
    const char *text_level = TextFormat("Lv%d", enemy->level);

    DrawText(text_name, 15, 15, font_size, BLACK);
    DrawText(text_level, game->screenWidth / 2 - MeasureText(text_level, font_size), 15, font_size, BLACK);

    DrawText("HP", rec_menu.width / 2 - MeasureText("HP", 20) - 5, rec_menu.y + font_size + 20, font_size, BLACK);
    draw_health_bar(enemy, rec_menu.width / 2, rec_menu.y + font_size + 20, rec_menu.width / 2, 15);
}

BattleEntidy create_entidy(Vector3 pos, const char *texture_path)
{
    BattleEntidy entidy;
    entidy.texture = LoadTexture(texture_path);
    entidy.position = pos;
    entidy.animation = (Animation){5, 0, 0.15f, 0.0f, false};

    return entidy;
}

Battle *init_battle(CombatStats *player, CombatStats *enemy)
{
    Battle *battle = calloc(1, sizeof(Battle));
    battle->enemy = create_entidy((Vector3){2, 1, -2}, "./assets/textures/enemies/bat/bat_battle.png");
    battle->enemy_data = enemy;
    battle->player = create_entidy((Vector3){-5, 1, 5}, "./assets/textures/snake/snake_battle.png");
    battle->player_data = player;

    Vector3 center = Vector3Add(
        battle->player.position,
        battle->enemy.position);

    center = Vector3Scale(center, -1.5f);
    battle->camera_battle.center = center;

    Vector3 dir = Vector3Normalize(
        Vector3Subtract(battle->camera_battle.center, battle->player.position));

    battle->camera_battle.cam.position =
        Vector3Subtract(
            battle->player.position,
            Vector3Scale(dir, 4.5f));

    battle->camera_battle.cam.position.y = 2.5f;

    battle->camera_battle.camera_radius = 6.0f;
    battle->camera_battle.camera_height = 2.5f;
    battle->camera_battle.camera_angle = PI * 0.75f;

    battle->camera_battle.cam.up = (Vector3){0, 1, 0};
    battle->camera_battle.cam.fovy = 45.0f;
    battle->camera_battle.cam.target = center;
    battle->camera_battle.cam.projection = CAMERA_PERSPECTIVE;

    return battle;
}

void update_animation(Animation *animation)
{
    if (animation->animation_timer > animation->frame_duration)
    {
        animation->animation_timer -= animation->frame_duration;

        if (animation->reverse == false)
            animation->animation_frame++;
        else
            animation->animation_frame--;

        if (animation->animation_frame >= animation->frame_count)
            animation->reverse = true;
        else if (animation->animation_frame <= 0)
            animation->reverse = false;
    }

    animation->animation_timer += GetFrameTime();
}
void draw_entidy(Battle *battleInfo)
{
    update_animation(&battleInfo->player.animation);
    float frameWidth = battleInfo->player.texture.width / battleInfo->player.animation.frame_count;
    float frameHeight = battleInfo->player.texture.height;

    Rectangle source = {
        (float)battleInfo->player.animation.animation_frame * frameWidth,
        0,
        (float)frameWidth,
        (float)frameHeight};

    DrawBillboardRec(
        battleInfo->camera_battle.cam,
        battleInfo->player.texture,
        source,
        battleInfo->player.position,
        (Vector2){1.5f, 1.5f},
        WHITE);
}