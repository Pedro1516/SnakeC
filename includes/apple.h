#include <raylib.h>
#include "player.h"
#include "game_state.h"

#ifndef APPLE_H
#define APPLE_H

typedef struct{
    Rectangle rec;
    Texture texture;
} Apple;

Apple create_apple(Rectangle rec);
void spawn_apple(Apple *apple, Game *game);
void draw_apple(Apple *apple, int apple_count);
void collect_apple(Player *player, Game *game, Apple *apple, int apple_count);

#endif