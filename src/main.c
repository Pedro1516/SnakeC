#include <raylib.h>
#include "../includes/player.h"
#include "../includes/input.h"
#include "../includes/apple.h"
#include "../includes/game_state.h"
#include <stdio.h>

int main(void)
{
    Game status = {
        .screenWidth = 800,
        .screenHeight = 600,
        .score = 0,
        .state = GAME_MENU};

    for (int i = 0; i < HIGHSCORE_MAX; i++)
        status.high_score[i].score = -1;

    status.high_score[0] = (HighScore){"Pedro", 10000};

    Player *player = create_player(WHITE, (Rectangle){status.screenWidth / 2 - 15, status.screenWidth / 2 - 15, 30, 30});
    Apple *apple = malloc(sizeof(Apple) * 5);
    apple[0] = create_apple((Rectangle){status.screenWidth / 4, status.screenWidth / 4, 30, 30}, RED);

    InitWindow(status.screenWidth, status.screenHeight, "Snake C");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Texture menu_logo = LoadTexture("assets/textures/title.png");

    float timer = 1;
    int index_menu = 0;

    Text text_highscore;
    text_highscore.character_inserted = 0;
    text_highscore.limit = 20;
    text_highscore.text = calloc(text_highscore.limit + 1, sizeof(char));

    while (!WindowShouldClose())
    {
        if (check_gameover(player, &status) && check_highscore(status))
            status.state = GAME_NEW_HIGHSCORE;

        if (IsKeyPressed(KEY_P) && (status.state == GAME_PLAYING || status.state == GAME_PAUSED))
            status.state = status.state == GAME_PAUSED ? GAME_PLAYING : GAME_PAUSED;

        if (IsKeyPressed(KEY_ESCAPE))
        {
            reset_game(&player, &status);
            status.state = GAME_MENU;
        }

        if (status.state == GAME_GAMEOVER)
        {
            timer += GetFrameTime();
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Game Over", status.screenWidth / 2 - MeasureText("Game Over", 40) / 2, status.screenHeight / 2 - 20, 40, RED);
            DrawText(TextFormat("Score: %d", status.score), status.screenWidth / 2 - MeasureText(TextFormat("Score: %d", status.score), 20) / 2, status.screenHeight / 2 + 20, 20, RAYWHITE);
            EndDrawing();

            if (timer >= 1.0f && timer < 4.0f)
            {
                DrawText("Press Any Key to Play Again", status.screenWidth / 2 - MeasureText("Press Any Key to Play Again", 20) / 2, status.screenHeight / 2 - 20 + 100, 20, RED);

                if (timer >= 2.0f)
                    timer = 0;
            }

            if (GetKeyPressed())
            {
                timer = 1;
                status.state = GAME_PLAYING;
                reset_game(&player, &status);
            }

            continue;
        }

        if (status.state == GAME_PLAYING)
        {
            move_player(player);
            ler_teclado(player);

            collect_apple(player, &status, apple, 1);
        }

        if (status.state == GAME_MENU)
        {
            timer += GetFrameTime();

            if (IsKeyPressed(KEY_DOWN))
                index_menu = (index_menu + 1) % 3;
            if (IsKeyPressed(KEY_UP))
                index_menu = index_menu <= 0 ? 2 : index_menu - 1;

            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(menu_logo, status.screenWidth / 2 - menu_logo.width / 2, 50, WHITE);
            draw_menu(status, index_menu);
            EndDrawing();

            if ((IsKeyPressed(KEY_ENTER)))
            {
                switch (index_menu)
                {
                case 0:
                    status.state = GAME_PLAYING;
                    break;
                case 1:
                    status.state = GAME_HIGHSCORE;
                    break;
                case 2:
                    status.state = GAME_SETTINGS;
                    break;

                default:
                    break;
                }
            }

            continue;
        }

        if (status.state == GAME_HIGHSCORE)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("HIGHSCORE", status.screenWidth / 2 - MeasureText("HIGHSCORE", 40) / 2, 10, 40, RED);
            EndDrawing();

            for (int i = 0; i < HIGHSCORE_MAX; i++)
            {
                if (status.high_score[i].score > 0)
                {
                    const char *text = TextFormat("%d. %s", i + 1, status.high_score[i].name);
                    int text_measure = MeasureText(text, 30);
                    DrawText(text, 20, 60 + i * 45, 30, RED);
                    DrawText(TextFormat("%d", status.high_score[i].score), 30 + text_measure, 60 + i * 45, 30, BLUE);
                }
                else
                    DrawText(TextFormat("%d.", i), 20, 60 + i * 45, 30, RED);
            }

            continue;
        }

        if (status.state == GAME_NEW_HIGHSCORE)
        {
            timer += GetFrameTime();

            read_keyboard(&text_highscore);
            draw_new_high_menu(&status, &timer, text_highscore.text);
            // new_highscore(&status, text_highscore.text);

            continue;
        }

        if (status.state == GAME_SETTINGS)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();

            continue;
        }

        BeginDrawing();

        ClearBackground(BLACK);
        draw_player(player);
        draw_apple(apple, 1);
        DrawText(TextFormat("Score: %d", status.score), 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    UnloadTexture(menu_logo);
    CloseWindow();

    return 0;
}