#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum
{
    GAME_MENU,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_GAMEOVER
} GameState;

typedef struct
{
    int screenWidth;
    int screenHeight;
    int score;
    int high_score;
    GameState state;
} Game;

#endif