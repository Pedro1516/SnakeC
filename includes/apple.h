#ifndef APPLE_H
#define APPLE_H

#include <raylib.h>
#include "player.h"
#include "game_state.h"

typedef struct
{
    Rectangle rec;
    Texture texture;
} Apple;

Apple create_apple(Rectangle rec);
void spawn_apple(Apple *apple, Game *game);
void draw_apple(Apple *apple, int apple_count, int debbugmode);
void collect_apple(Player *player, Game *game, Apple *apple, int apple_count);

#endif