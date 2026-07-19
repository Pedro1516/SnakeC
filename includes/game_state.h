#include <string.h>
#include <string.h>

#ifndef GAME_STATE_H
#define GAME_STATE_H
#define HIGHSCORE_MAX 10

typedef struct Player Player;

typedef enum
{
    GAME_MENU,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_GAMEOVER,
    GAME_HIGHSCORE,
    GAME_NEW_HIGHSCORE,
    GAME_SETTINGS
} GameState;

typedef struct
{
    const char *name;
    int score;
} HighScore;

typedef struct Game
{
    int screenWidth;
    int screenHeight;
    int score;
    HighScore high_score[HIGHSCORE_MAX];
    GameState state;
} Game;

void new_highscore(Game *game, const char *name);
void reset_game(Player **player, Game *game);
char *string_duplicate(const char *str);
void draw_menu(Game game, int index_menu);
int check_highscore(Game game);

#endif