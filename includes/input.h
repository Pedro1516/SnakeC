#ifndef INPUT_H
#define INPUT_H

#include "player.h"
#include "types.h"

typedef struct Text
{
    char *text;
    int limit;
    int character_inserted;
} Text;

void read_movement_player(Snake *snake);
void read_keyboard(Text *text);
void reset_text(Text *text);

#endif