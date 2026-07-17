#include <raylib.h>
#include "../includes/player.h"
#include "../includes/input.h"
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

int main(void)
{
    Player *player = create_player(WHITE, (Rectangle){WIDTH / 2 - 15, HEIGHT / 2 - 15, 30, 30});
    add_node(player);
    add_node(player);
    add_node(player);
    add_node(player);
    add_node(player);
    add_node(player);

    InitWindow(800, 600, "Snake C");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        move_player(player);
        ler_teclado(player);

        BeginDrawing();

        ClearBackground(BLACK);
        draw_player(player);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}