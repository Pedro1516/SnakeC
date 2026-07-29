#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../includes/player.h"
#include "../includes/input.h"
#include "../includes/apple.h"
#include "../includes/game_state.h"
#include "../includes/queue.h"
#include "../includes/grass.h"
#include "../includes/battle.h"
#include "../includes/enemy.h"
#include "../includes/combat_stats.h"

void update_menu(float timer, int *index_menu, Texture *menu_background, Texture *menu_logo, Game *game, Player **player);
void update_highscore(Game game);
void update_new_highscore(float timer, Game *game, Player **player, Text *text_highscore);
void update_gameover(float *timer, Game *game, Player **player);
void update_playing(Grass *grass, Apple *apple, Game *game, Player *player, int *debbugmode, float timer);

int main(void)
{
    int debbugmode = 0;
    Game status = {
        .screenWidth = 800,
        .screenHeight = 600,
        .score = 0,
        .state = GAME_MENU};

    printf("%s\n", RAYLIB_VERSION);
    InitWindow(status.screenWidth, status.screenHeight, "Snake C");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    for (int i = 0; i < HIGHSCORE_MAX; i++)
    {
        status.high_score[i].score = -1;
        status.high_score[i].name = NULL;
    }

    HighScore *highscore = read_file();
    load_highscore(highscore, status.high_score);
    free(highscore);

    Player *player = create_player((Rectangle){status.screenWidth / 2 - 15, status.screenWidth / 2 - 15, 30, 30});
    Grass *grass = init_grass(WIND_RIGHT);
    Apple *apple = malloc(sizeof(Apple) * 5);
    apple[0] = create_apple((Rectangle){status.screenWidth / 4, status.screenWidth / 4, 30, 30});

    status.font = LoadFontEx("assets/fonts/EduVICWANTHand-VariableFont_wght.ttf", 60, NULL, 0);
    SetTextureFilter(status.font.texture, TEXTURE_FILTER_BILINEAR);

    Texture menu_logo = LoadTexture("assets/textures/title/title.png");
    Texture menu_background = LoadTexture("assets/textures/title/title_background.png");

    load_texture_grass(grass, "assets/textures/grass/Grass_Right1.png");
    load_texture_grass(grass, "assets/textures/grass/Grass_Right2.png");
    load_texture_grass(grass, "assets/textures/grass/Grass_Right3.png");

    float timer = 0;
    int index_menu = 0;

    Text text_highscore;
    text_highscore.character_inserted = 0;
    text_highscore.limit = 20;
    text_highscore.text = calloc(text_highscore.limit + 1, sizeof(char));

    Enemy enemy = {(CombatStats){100, 100, 10, 15, 50, 2}, "Bat", 15};
    Battle *battle = init_battle(&(player->status), &(enemy.status));

    float frame_target = 0;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        timer += dt;
        if (status.state == GAME_CLOSE)
            break;

        if (check_gameover(&player->snake, &status) && check_highscore(status))
            status.state = GAME_NEW_HIGHSCORE;

        if (IsKeyPressed(KEY_ESCAPE) && !(status.state == GAME_NEW_HIGHSCORE))
        {
            if (status.state == GAME_PLAYING || status.state == GAME_PAUSED)
            {
                status.state = status.state == GAME_PAUSED ? GAME_PLAYING : GAME_PAUSED;
            }

            else
                status.state = GAME_MENU;
        }

        switch (status.state)
        {
        case GAME_MENU:
            update_menu(timer, &index_menu, &menu_background, &menu_logo, &status, &player);
            break;
        case GAME_GAMEOVER:
            update_gameover(&timer, &status, &player);
            break;
        case GAME_HIGHSCORE:
            update_highscore(status);
            break;
        case GAME_NEW_HIGHSCORE:
            update_new_highscore(timer, &status, &player, &text_highscore);
            break;
        case GAME_SETTINGS:
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
            continue;
        case GAME_PAUSED:
        case GAME_PLAYING:
            update_playing(grass, apple, &status, player, &debbugmode, timer);
            break;
        case GAME_BATTLE:
            // frame_target += dt;

            // if (frame_target >= 1.5)
            // {
            //     frame_target -= 1.5;

            //     combat_take_damage(&(enemy.status), 25);
            // }

            BeginDrawing();
            ClearBackground(BLUE);

            BeginMode3D(battle->camera_battle.cam);

            DrawPlane((Vector3){0, 0, 0},
                      (Vector2){20, 20},
                      DARKGREEN);

            DrawBillboard(
                battle->camera_battle.cam,
                battle->enemy.texture,
                battle->enemy.position,
                1.5f,
                WHITE);

            draw_entidy(battle);

            EndMode3D();
            draw_info_enemy(&status, &enemy);
            DrawText(TextFormat("timer: %.2f", timer), status.screenWidth - MeasureText(TextFormat("timer: %.2f", timer), 30), 10, 30, RED);

            EndDrawing();
            break;
        default:
            break;
        }

        if (timer >= 60)
            timer = 0;
    }

    save_highscore(status.high_score);
    UnloadTexture(menu_logo);
    UnloadTexture(menu_background);
    CloseWindow();

    return 0;
}

#define MENU_ITEMS 4

void update_menu(float timer, int *index_menu, Texture *menu_background, Texture *menu_logo, Game *game, Player **player)
{

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        *index_menu = (*index_menu + 1) % MENU_ITEMS;

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        *index_menu = (*index_menu == 0) ? MENU_ITEMS - 1 : *index_menu - 1;

    Rectangle menuRect[MENU_ITEMS];

    BeginDrawing();

    ClearBackground(BLACK);

    DrawTextureEx(
        *menu_background,
        (Vector2){0, 0},
        0,
        1.5f,
        (Color){255, 255, 255, 50});

    DrawTexture(
        *menu_logo,
        game->screenWidth / 2 - menu_logo->width / 2,
        50,
        WHITE);

    // Primeiro calcula e desenha o menu
    draw_menu(*game, menuRect);

    // Depois verifica o mouse
    bool hover = select_menu(menuRect, index_menu, MENU_ITEMS);

    // Desenha o destaque
    Color border = animate_color_sin(timer, 0.1f, 5.0f, YELLOW);

    DrawRectangleRoundedLines(
        menuRect[*index_menu],
        0.25f,
        8,
        border);

    EndDrawing();

    if (IsKeyPressed(KEY_ENTER) || (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
    {
        switch (*index_menu)
        {
        case 0:
            game->state = GAME_PLAYING;
            reset_game(player, game);
            break;

        case 1:
            game->state = GAME_HIGHSCORE;
            break;

        case 2:
            game->state = GAME_SETTINGS;
            break;

        case 3:
            game->state = GAME_CLOSE;
            break;
        }
    }
}

void update_highscore(Game game)
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("HIGHSCORE", game.screenWidth / 2 - MeasureText("HIGHSCORE", 40) / 2, 10, 40, RED);

    for (int i = 0; i < HIGHSCORE_MAX; i++)
    {
        if (game.high_score[i].score > 0)
        {
            const char *text = TextFormat("%d. %s", i + 1, game.high_score[i].name);
            int text_measure = MeasureText(text, 30);
            DrawText(text, 20, 60 + i * 45, 30, RED);
            DrawText(TextFormat("%d", game.high_score[i].score), 30 + text_measure, 60 + i * 45, 30, BLUE);
        }
        else
            DrawText(TextFormat("%d.", i + 1), 20, 60 + i * 45, 30, RED);
    }

    EndDrawing();
}

void update_new_highscore(float timer, Game *game, Player **player, Text *text_highscore)
{

    read_keyboard(text_highscore);
    draw_new_high_menu(game, timer, text_highscore->text);

    if (IsKeyPressed(KEY_ENTER) && text_highscore->character_inserted > 0)
    {
        new_highscore(game, text_highscore->text);
        game->state = GAME_PLAYING;
        reset_game(player, game);
        reset_text(text_highscore);
    }
}

void update_gameover(float *timer, Game *game, Player **player)
{
    *timer += GetFrameTime();
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Game Over", game->screenWidth / 2 - MeasureText("Game Over", 40) / 2, game->screenHeight / 2 - 20, 40, RED);
    DrawText(TextFormat("Score: %d", game->score), game->screenWidth / 2 - MeasureText(TextFormat("Score: %d", game->score), 20) / 2, game->screenHeight / 2 + 20, 20, RAYWHITE);
    EndDrawing();

    float alpha = (sinf(*timer * 5.0f) + 1.0f) * 0.5f;
    Color color = Fade(RED, alpha);
    DrawText("Press Any Key to Play Again", game->screenWidth / 2 - MeasureText("Press Any Key to Play Again", 20) / 2, game->screenHeight / 2 - 20 + 100, 20, color);

    if (GetKeyPressed())
    {
        *timer = 1;
        game->state = GAME_PLAYING;
        reset_game(player, game);
    }
}

int check_click_menu(Rectangle *rects, int rects_count, float timer)
{
    Vector2 mousepoint = GetMousePosition();

    for (int i = 0; i < rects_count; i++)
    {
        if (CheckCollisionPointRec(mousepoint, rects[i]))
        {
            Color border_color = animate_color_sin(timer, 0.0f, 10.0f, YELLOW);
            Rectangle rect_border = {rects[i].x - 2, rects[i].y - 2, rects[i].width + 4, rects[i].height + 4};
            DrawRectangleRoundedLines(rect_border, 0.25f, 10, border_color);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                return i;
            }
        }
    }
    return -1;
}

void update_paused(Game *game, float timer)
{
    int options_count = 4;
    int font_size = 35;
    char *options[options_count];
    options[0] = "Resume";
    options[1] = "Inventory";
    options[2] = "Settings";
    options[3] = "Main Menu";

    Rectangle options_rects[options_count];
    Rectangle menu_rect = {10, 10, 300, 350};
    DrawRectangleRec(menu_rect, (Color){0, 0, 127, 200});
    DrawRectangleLines(menu_rect.x - 1, menu_rect.y - 1, menu_rect.width + 2, menu_rect.height + 2, RED);

    DrawText("Game Paused", menu_rect.x + menu_rect.width / 2 - MeasureText("Game Paused", font_size + 5) / 2, menu_rect.y + 10, font_size + 5, BLACK);

    int top_padding = 10 + 2 * font_size;
    int left_padding = menu_rect.width / 2;
    int v_spacing = 10;

    for (int i = 0; i < options_count; i++)
    {
        int text_width = MeasureText(options[i], font_size);

        DrawText(options[i], menu_rect.x + left_padding - text_width / 2, (menu_rect.y + top_padding + i * font_size) + v_spacing * i, font_size, BLACK);
        options_rects[i].x = menu_rect.x + left_padding - text_width / 2;
        options_rects[i].y = (menu_rect.y + top_padding + i * font_size + v_spacing * i);
        options_rects[i].width = text_width;
        options_rects[i].height = font_size;
    }

    int selected_option = check_click_menu(options_rects, options_count, timer);

    switch (selected_option)
    {
    case 0:
        game->state = GAME_PLAYING;
        break;
    case 3:
        game->state = GAME_MENU;
        break;

    default:
        break;
    }
}

void update_playing(Grass *grass, Apple *apple, Game *game, Player *player, int *debbugmode, float timer)
{
    if (IsKeyPressed(KEY_P))
        *debbugmode = !(*debbugmode);

    if (game->state != GAME_PAUSED)
    {
        move_player(&player->snake);
        read_movement_player(&player->snake);
        collect_apple(&player->snake, game, apple, 1);
        update_grass(grass);
    }

    BeginDrawing();

    ClearBackground((Color){34, 177, 76, 255});
    draw_grass(grass);
    draw_apple(apple, 1, *debbugmode);
    draw_player(player, *debbugmode);
    DrawText(TextFormat("Score: %d", game->score), 10, 10, 20, RAYWHITE);

    if (game->state == GAME_PAUSED)
        update_paused(game, timer);
    EndDrawing();
}