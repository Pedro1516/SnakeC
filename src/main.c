#include <raylib.h>
#include "../includes/player.h"

#define WIDTH 800
#define HEIGHT 600


int main(void)
{
    Player *player = create_player(WHITE, (Rectangle){WIDTH / 2 - 15, HEIGHT / 2 - 15, 30, 30});

    InitWindow(800, 600, "Teste");

    SetTargetFPS(60);


    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        move_player(player, dt);
        
        BeginDrawing();

        ClearBackground(BLACK);
        draw_player(player);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}