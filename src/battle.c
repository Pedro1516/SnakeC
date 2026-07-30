#include "../includes/enemy.h"
#include "../includes/battle.h"
#include "../includes/game_state.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

void draw_health_bar(const CombatStats *status, int posX, int posY, int width, int height, Texture *texture)
{
    Rectangle rec_background = {posX, posY, width, height};
    Rectangle rec_heath = rec_background;
    rec_heath.width = status->health >= 0 ? (width * status->health) / status->max_health : 0;

    DrawRectangleRounded(rec_background, 0.5f, 10, LIGHTGRAY); // Background
    DrawRectangleRounded(rec_heath, 0.5f, 10, RED);            // Health
    // DrawTexture(*texture, posX - 34, posY - 5, WHITE);
}

void draw_info_enemy(const Game *game, const Enemy *enemy, Texture *texture_bar)
{
    Rectangle rec_menu = {
        10,
        10,
        game->screenWidth / 2,
        75,
    };

    DrawRectangleRounded(rec_menu, 0.4f, 30, RAYWHITE);
    DrawRectangleRoundedLinesEx((Rectangle){rec_menu.x, rec_menu.y, rec_menu.width, rec_menu.height}, 0.4f, 30, 2, BLACK);

    int font_size = 20;
    const char *text_name = enemy->name;
    const char *text_level = TextFormat("Lv%d", enemy->level);

    DrawText(text_name, 25, 25, font_size, BLACK);
    DrawText(text_level, game->screenWidth / 2 - MeasureText(text_level, font_size) - 15, 25, font_size, BLACK);

    draw_health_bar(&(enemy->status), rec_menu.width / 2 - 15, rec_menu.y + font_size + 20, rec_menu.width / 2, 15, texture_bar);
}

BattleEntidy create_entidy(Vector3 pos, const char *texture_path)
{
    BattleEntidy entidy;
    entidy.texture = LoadTexture(texture_path);
    entidy.position = pos;
    entidy.animation = (Animation){5, 0, 0.15f, 0.0f, false};

    return entidy;
}

Battle *init_battle(CombatStats *player, CombatStats *enemy, const char *texture_player_path, const char *texture_enemy_path)
{
    Battle *battle = calloc(1, sizeof(Battle));
    battle->enemy = create_entidy((Vector3){2, 1, -2}, texture_enemy_path);
    battle->enemy_data = enemy;
    battle->player = create_entidy((Vector3){-5, 1, 5}, texture_player_path);
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
            Vector3Scale(dir, 2.0f));

    battle->camera_battle.cam.position.y = 2.5f;
    battle->camera_battle.cam.position.x -= 2.5f;

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

        if (animation->animation_frame >= animation->frame_count - 1)
            animation->reverse = true;
        else if (animation->animation_frame <= 0)
            animation->reverse = false;
    }
    animation->animation_timer += GetFrameTime();
}

void draw_entidy(Battle *battleInfo)
{
    update_animation(&battleInfo->player.animation);
    update_animation(&battleInfo->enemy.animation);
    float frameWidth_player = battleInfo->player.texture.width / battleInfo->player.animation.frame_count;
    float frameHeight_player = battleInfo->player.texture.height;

    float frameWidth_enemy = battleInfo->enemy.texture.width / battleInfo->enemy.animation.frame_count;
    float frameHeight_enemy = battleInfo->enemy.texture.height;

    Rectangle source_player = {
        (float)battleInfo->player.animation.animation_frame * frameWidth_player,
        0,
        (float)frameWidth_player,
        (float)frameHeight_player};

    Rectangle source_enemy = {
        (float)battleInfo->enemy.animation.animation_frame * frameWidth_enemy,
        0,
        (float)frameWidth_enemy,
        (float)frameHeight_enemy};

    if (!combat_is_dead(battleInfo->player_data))
        DrawBillboardRec(
            battleInfo->camera_battle.cam,
            battleInfo->player.texture,
            source_player,
            battleInfo->player.position,
            (Vector2){1.5f, 1.5f},
            WHITE);

    if (!combat_is_dead(battleInfo->enemy_data))
        DrawBillboardRec(
            battleInfo->camera_battle.cam,
            battleInfo->enemy.texture,
            source_enemy,
            battleInfo->enemy.position,
            (Vector2){2, 2},
            WHITE);
}

void draw_menu_player(const CombatStats *player, const Game *game, float timer, int *index_menu, Rectangle *rect_options)
{
    Rectangle rect_menu = {10, game->screenHeight - 210, 400, 200};
    DrawRectangleRounded(rect_menu, 0.2f, 10, RAYWHITE);

    int text_count = 5;
    int font_size = 20;
    int v_spacing = font_size + 10;
    const char *text[text_count];
    text[0] = "Atacar";
    text[1] = "Habilidade";
    text[2] = "Inventário";
    text[3] = "Status";
    text[4] = "Fugir";

    for (int i = 0; i < text_count; i++)
    {
        const int text_width = MeasureText(text[i], font_size);
        const int posX = rect_menu.width - text_width / 2 - text_width;
        const int posY = rect_menu.y + 25 + i * v_spacing;

        DrawText(text[i], posX, posY, font_size, BLACK);
        rect_options[i] = (Rectangle){posX - 5, posY - 5, text_width + 10, font_size + 10};
    }

    draw_health_bar(player, rect_menu.x + 50, rect_menu.y + 10, 200, 15, (Texture *){0});
    const char *health = TextFormat("%d/%d", 100, 100);
    DrawText(health, rect_menu.x + 50 + 200 - MeasureText(health, 10), rect_menu.y + 10 + 25, 10, BLACK);
}