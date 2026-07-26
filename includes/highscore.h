#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#define HIGHSCORE_MAX 10

#include "types.h"

typedef struct
{
    const char *name;
    int score;
} HighScore;

HighScore *read_file();
int load_highscore(HighScore *highscore_to_load, HighScore *highscore_game);
int save_highscore(HighScore *highscore);
void new_highscore(Game *game, const char *name);

#endif