#include "../includes/input.h"

void ler_teclado(Player *player)
{
    if (IsKeyPressed(KEY_S) && player->head->direction.y != 1 && player->head->direction.y != -1)
    {
        player->head->direction = (Vector2){0, -1};
        register_curve(player);
    }
    if (IsKeyPressed(KEY_W) && player->head->direction.y != -1 && player->head->direction.y != 1)
    {
        player->head->direction = (Vector2){0, 1};
        register_curve(player);
    }
    if (IsKeyPressed(KEY_D) && player->head->direction.x != 1 && player->head->direction.x != -1)
    {
        player->head->direction = (Vector2){-1, 0};
        register_curve(player);
    }
    if (IsKeyPressed(KEY_A) && player->head->direction.x != -1 && player->head->direction.x != 1)
    {
        player->head->direction = (Vector2){1, 0};
        register_curve(player);
    }
}