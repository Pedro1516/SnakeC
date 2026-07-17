#include <raylib.h>
#include "../includes/player.h"
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

int main(void)
{
    Player *player = create_player(WHITE, (Rectangle){WIDTH / 2 - 15, HEIGHT / 2 - 15, 30, 30});
    add_node(player);
    add_node(player);

    InitWindow(800, 600, "Snake C");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        move_player(player, dt);
        
        if (IsKeyPressed(KEY_S))
            player->head->direction = (Vector2){0, 1};
        if (IsKeyPressed(KEY_W))
            player->head->direction = (Vector2){0, -1};

        BeginDrawing();

        ClearBackground(BLACK);
        draw_player(player);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}