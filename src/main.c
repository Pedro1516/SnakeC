#include <raylib.h>


#define WIDTH 800
#define HEIGHT 600


int main(void)
{
    InitWindow(800, 600, "Teste");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}