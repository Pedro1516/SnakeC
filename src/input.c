#include "../includes/input.h"


void read_movement_player(Player *player)
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


void read_keyboard(Text *text)
{
    int key = GetKeyPressed();

    if (key > 0)
    {
        if (text->character_inserted < text->limit)
        {
            if ((key >= 32 && key <= 93))
            {
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