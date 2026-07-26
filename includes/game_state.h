#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <raylib.h>
#include "highscore.h"
#include "types.h"
#define MENU_ITEMS 4

typedef enum
{
    GAME_MENU,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_GAMEOVER,
    GAME_HIGHSCORE,
    GAME_NEW_HIGHSCORE,
    GAME_SETTINGS,
    GAME_CLOSE,
    GAME_BATTLE
} GameState;

struct Game
{
    int screenWidth;
    int screenHeight;
    int score;
    Font font;
    HighScore high_score[HIGHSCORE_MAX];
    GameState state;
};

typedef struct Animation
{
    int frame_count;
    int animation_frame;
    float frame_duration;
    float animation_timer;
} Animation;

void reset_game(Player **player, Game *game);
char *string_duplicate(const char *str);
void draw_menu(Game game, Rectangle rec[]);
int check_highscore(Game game);
void draw_new_high_menu(Game *game, float *timer, const char *text);
bool select_menu(Rectangle *rec, int *index_menu, int btn_count);

#endif