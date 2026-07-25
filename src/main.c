#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../includes/player.h"
#include "../includes/input.h"
#include "../includes/apple.h"
#include "../includes/game_state.h"
#include "../includes/queue.h"
#include "../includes/grass.h"

void update_menu(float *timer, int *index_menu, Texture *menu_background, Texture *menu_logo, Game *game);
void update_highscore(Game game);
void update_new_highscore(float *timer, Game *game, Player **player, Text *text_highscore);
void update_gameover(float *timer, Game *game, Player **player);
void update_playing(Grass *grass, Apple *apple, Game *game, Player *player);

int main(void)
{
    Game status = {
        .screenWidth = 800,
        .screenHeight = 600,
        .score = 0,
        .state = GAME_MENU,

    };

    InitWindow(status.screenWidth, status.screenHeight, "Snake C");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    for (int i = 0; i < HIGHSCORE_MAX; i++)
        status.high_score[i].score = -1;

    HighScore *highscore = read_file();
    load_highscore(highscore, status.high_score);
    free(highscore);

    Player *player = create_player((Rectangle){status.screenWidth / 2 - 15, status.screenWidth / 2 - 15, 30, 30});
    Grass *grass = init_grass(WIND_RIGHT);
    Apple *apple = malloc(sizeof(Apple) * 5);
    apple[0] = create_apple((Rectangle){status.screenWidth / 4, status.screenWidth / 4, 30, 30});

    status.font = LoadFontEx(
        "assets/fonts/EduVICWANTHand-VariableFont_wght.ttf",
        60,
        NULL,
        0);
    SetTextureFilter(status.font.texture, TEXTURE_FILTER_BILINEAR);

    Texture menu_logo = LoadTexture("assets/textures/title/title.png");
    Texture menu_background = LoadTexture("assets/textures/title/title_background.png");

    load_texture_grass(grass, "assets/textures/grass/Grass_Right1.png");
    load_texture_grass(grass, "assets/textures/grass/Grass_Right2.png");
    load_texture_grass(grass, "assets/textures/grass/Grass_Right3.png");

    float timer = 1;
    int index_menu = 0;

    Text text_highscore;
    text_highscore.character_inserted = 0;
    text_highscore.limit = 20;
    text_highscore.text = calloc(text_highscore.limit + 1, sizeof(char));

    while (!WindowShouldClose())
    {
        if (status.state == GAME_CLOSE)
            break;

        if (check_gameover(player, &status) && check_highscore(status))
            status.state = GAME_NEW_HIGHSCORE;

        if (IsKeyPressed(KEY_P) && (status.state == GAME_PLAYING || status.state == GAME_PAUSED))
            status.state = status.state == GAME_PAUSED ? GAME_PLAYING : GAME_PAUSED;

        if (IsKeyPressed(KEY_ESCAPE))
        {
            reset_game(&player, &status);
            status.state = GAME_MENU;
        }

        switch (status.state)
        {
        case GAME_MENU:
            update_menu(&timer, &index_menu, &menu_background, &menu_logo, &status);
            break;
        case GAME_GAMEOVER:
            update_gameover(&timer, &status, &player);
            break;
        case GAME_HIGHSCORE:
            update_highscore(status);
            break;
        case GAME_NEW_HIGHSCORE:
            update_new_highscore(&timer, &status, &player, &text_highscore);
            break;
        case GAME_SETTINGS:
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
            continue;
        case GAME_PLAYING:
            update_playing(grass, apple, &status, player);
            break;
        default:
            break;
        }
    }

    save_highscore(status.high_score);
    UnloadTexture(menu_logo);
    UnloadTexture(menu_background);
    CloseWindow();

    return 0;
}

void update_menu(float *timer, int *index_menu, Texture *menu_background, Texture *menu_logo, Game *game)
{
    (*timer) += GetFrameTime();

    if (IsKeyPressed(KEY_DOWN))
        (*index_menu) = ((*index_menu) + 1) % 4;
    if (IsKeyPressed(KEY_UP))
        (*index_menu) = (*index_menu) <= 0 ? 3 : (*index_menu) - 1;

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextureEx((*menu_background), (Vector2){0, 0}, 0, 1.5, (Color){255, 255, 255, 50});
    DrawTexture((*menu_logo), game->screenWidth / 2 - (*menu_logo).width / 2, 50, WHITE);
    draw_menu((*game), (*index_menu));
    EndDrawing();

    if ((IsKeyPressed(KEY_ENTER)))
    {
        switch ((*index_menu))
        {
        case 0:
            (*game).state = GAME_PLAYING;
            break;
        case 1:
            (*game).state = GAME_HIGHSCORE;
            break;
        case 2:
            (*game).state = GAME_SETTINGS;
            break;
        case 3:
            (*game).state = GAME_CLOSE;
        default:
            break;
        }
    }
}

void update_highscore(Game game)
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("HIGHSCORE", game.screenWidth / 2 - MeasureText("HIGHSCORE", 40) / 2, 10, 40, RED);
    EndDrawing();

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
            DrawText(TextFormat("%d.", i), 20, 60 + i * 45, 30, RED);
    }
}

void update_new_highscore(float *timer, Game *game, Player **player, Text *text_highscore)
{
    *timer += GetFrameTime();

    read_keyboard(text_highscore);
    draw_new_high_menu(game, timer, text_highscore->text);

    if (IsKeyPressed(KEY_ENTER) && text_highscore->character_inserted > 0)
    {
        new_highscore(game, text_highscore->text);
        game->state = GAME_MENU;
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

    if (*timer >= 1.0f && *timer < 4.0f)
    {
        DrawText("Press Any Key to Play Again", game->screenWidth / 2 - MeasureText("Press Any Key to Play Again", 20) / 2, game->screenHeight / 2 - 20 + 100, 20, RED);

        if (*timer >= 2.0f)
            *timer = 0;
    }

    if (GetKeyPressed())
    {
        *timer = 1;
        game->state = GAME_PLAYING;
        reset_game(player, game);
    }
}

void update_playing(Grass *grass, Apple *apple, Game *game, Player *player)
{
    move_player(player);
    read_movement_player(player);

    collect_apple(player, game, apple, 1);
    update_grass(grass);

    BeginDrawing();

    ClearBackground((Color){34, 177, 76, 255});
    draw_grass(grass);
    draw_apple(apple, 1);
    draw_player(player);
    DrawText(TextFormat("Score: %d", game->score), 10, 10, 20, RAYWHITE);
    EndDrawing();
}