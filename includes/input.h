#include "player.h"
#ifndef INPUT_H
#define INPUT_H

typedef struct Text
{
    char *text;
    int limit;
    int character_inserted;
} Text;

void read_movement_player(Player *player);
void read_keyboard(Text *text);
void reset_text(Text *text);

#endif