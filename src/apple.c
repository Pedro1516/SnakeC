
#include "../includes/apple.h"

Apple create_apple(Rectangle rec, Color color)
{
    Apple a;
    a.rec = rec;
    a.color = color;

    return a;
}

void spawn_apple(Apple *apple, Game *game)
{
    apple->rec.x = GetRandomValue(0, game->screenWidth - apple->rec.width);
    apple->rec.y = GetRandomValue(0, game->screenHeight - apple->rec.height);
}

void draw_apple(Apple *apple, int apple_count)
{
    for (int i = 0; i < apple_count; i++)
    {
        DrawRectangleRec(apple->rec, apple->color);
    }
}

void collect_apple(Player *player, Game *game, Apple *apple, int apple_count)
{
    for (int i = 0; i < apple_count; i++)
    {
        if (CheckCollisionRecs(player->head->rect, apple[i].rec))
        {
            scoreup(player, game);
            spawn_apple(&apple[i], game);
        }
    }
}
