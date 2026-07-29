
#include "../includes/apple.h"

Apple create_apple(Rectangle rec)
{
    Apple a;
    a.rec = rec;
    a.texture = LoadTexture("assets/textures/apple/apple.png");

    return a;
}

void spawn_apple(Apple *apple, Game *game)
{
    apple->rec.x = GetRandomValue(0, game->screenWidth - apple->rec.width);
    apple->rec.y = GetRandomValue(0, game->screenHeight - apple->rec.height);
}

void draw_apple(Apple *apple, int apple_count, int debbugmode)
{
    for (int i = 0; i < apple_count; i++)
    {
        DrawTexture(apple[i].texture, apple[i].rec.x + 3, apple[i].rec.y + 5, (Color){0, 0, 0, 127});
        DrawTexture(apple[i].texture, apple[i].rec.x, apple[i].rec.y, WHITE);
        if (debbugmode)
            DrawRectangleLines(apple[i].rec.x, apple[i].rec.y, apple[i].rec.width, apple[i].rec.height, MAGENTA);
    }
}

void collect_apple(Snake *snake, Game *game, Apple *apple, int apple_count)
{
    for (int i = 0; i < apple_count; i++)
    {
        if (CheckCollisionRecs(snake->head->rect, apple[i].rec))
        {
            scoreup(snake, game);
            spawn_apple(&apple[i], game);
        }
    }
}
