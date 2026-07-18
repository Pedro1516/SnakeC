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
        .high_score = 0,
        .state = GAME_MENU};

    Player *player = create_player(WHITE, (Rectangle){status.screenWidth / 2 - 15, status.screenWidth / 2 - 15, 30, 30});
    Apple *apple = malloc(sizeof(Apple) * 5);
    apple[0] = create_apple((Rectangle){status.screenWidth / 4, status.screenWidth / 4, 30, 30}, RED);

    InitWindow(status.screenWidth, status.screenHeight, "Snake C");
    Texture menu_logo = LoadTexture("assets/textures/title.png");

    SetTargetFPS(60);

    float timer = 1;
    while (!WindowShouldClose())
    {
        check_gameover(player, &status);

        if (status.state == GAME_PLAYING)
        {
            move_player(player);
            ler_teclado(player);

            collect_apple(player, &status, apple, 1);
        }

        if (status.state == GAME_MENU)
        {
            timer += GetFrameTime();
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(menu_logo, status.screenWidth / 2 - menu_logo.width / 2, 50, WHITE);
            EndDrawing();

            if (timer >= 1.0f && timer < 5.0f)
            {
                DrawText("Jogar", status.screenWidth / 2 - MeasureText("Jogar", 40) / 2, status.screenHeight / 2 - 20 + 100, 40, RED);

                if (timer >= 3.0f)
                    timer = 0;
            }

            if (GetKeyPressed() || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                status.state = GAME_PLAYING;
                
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