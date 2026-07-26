#include "../includes/input.h"
#include <raymath.h>

void read_movement_player(Player *player)
{
    Vector2 pos_head = (Vector2){player->head->rect.x, player->head->rect.y};
    Vector2 pos_node2 = (Vector2){player->head->prox->rect.x, player->head->prox->rect.y};
    float dis = Vector2Distance(pos_head, pos_node2);

    int key = GetKeyPressed();
    switch (key)
    {
    case KEY_W:
    case KEY_UP:
        if (player->head->direction.y == 0 && dis >= player->head->rect.width)
        {
            player->head->direction = (Vector2){0, 1};
            register_curve(player);
        }
        break;
    case KEY_S:
    case KEY_DOWN:
        if (player->head->direction.y == 0 && dis >= player->head->rect.width)
        {
            player->head->direction = (Vector2){0, -1};
            register_curve(player);
        }
        break;
    case KEY_A:
    case KEY_LEFT:
        if (player->head->direction.x == 0 && dis >= player->head->rect.width)
        {
            player->head->direction = (Vector2){1, 0};
            register_curve(player);
        }
        break;
    case KEY_D:
    case KEY_RIGHT:
        if (player->head->direction.x == 0 && dis >= player->head->rect.width)
        {
            player->head->direction = (Vector2){-1, 0};
            register_curve(player);
        }
        break;
    }
}

void read_keyboard(Text *text)
{
    int key = GetKeyPressed();
    bool upercase = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? true : false;

    if (key > 0)
    {
        if (text->character_inserted < text->limit)
        {
            if ((key >= 32 && key <= 93))
            {
                if (!upercase && key >= 65 && key <= 90)
                    key += 32;
                text->text[text->character_inserted] = key;
                text->character_inserted++;
            }
        }

        if (text->character_inserted > 0 && key == KEY_BACKSPACE)
        {
            text->character_inserted--;
            text->text[text->character_inserted] = 0;
        }
    }
}

void reset_text(Text *text)
{
    text->character_inserted = 0;
    for (int i = 0; i < text->limit; i++)
    {
        text->text[i] = (char)0;
    }
}