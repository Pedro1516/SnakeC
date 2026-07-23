#include <raylib.h>
#include "highscore.h"

#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef struct Player Player;


typedef enum
{
    GAME_MENU,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_GAMEOVER,
    GAME_HIGHSCORE,
    GAME_NEW_HIGHSCORE,
    GAME_SETTINGS,
    GAME_CLOSE
} GameState;

typedef struct Game
{
    int screenWidth;
    int screenHeight;
    int score;
    Font font;
    HighScore high_score[HIGHSCORE_MAX];
    GameState state;
} Game;


void reset_game(Player **player, Game *game);
char *string_duplicate(const char *str);
void draw_menu(Game game, int index_menu);
int check_highscore(Game game);
void draw_new_high_menu(Game *game, float *timer, const char *text);


#endif