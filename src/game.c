#include "../includes/game_state.h"
#include "../includes/player.h"
#include <stdlib.h>
#include <string.h>

void new_highscore(Game *game, const char *name)
{
    HighScore new_highsore;
    new_highsore.name = string_duplicate(name);
    new_highsore.score = game->score;
    for (int i = 0; i < HIGHSCORE_MAX; i++)
    {
        if (game->score > game->high_score[i].score)
        {
            for (int j = HIGHSCORE_MAX - 1; j > i; j--)
            {
                game->high_score[j] = game->high_score[j - 1];
            }

            game->high_score[i] = new_highsore;
            return;
        }
    }
}

void reset_game(Player **player, Game *game)
{
    kill_player(*player);
    *player = create_player((Rectangle){game->screenWidth / 2 - 15, game->screenWidth / 2 - 15, 30, 30});
    game->score = 0;
}

char *string_duplicate(const char *str)
{
    char *copy = malloc(strlen(str) + 1);

    if (copy != NULL)
        strcpy(copy, str);

    return copy;
}

void draw_menu(Game game, Rectangle rec[])
{
    const char *text[MENU_ITEMS] = {
        "Play",
        "Highscore",
        "Settings",
        "Quit"};

    const int fontSize = 60;

    for (int i = 0; i < MENU_ITEMS; i++)
    {
        Vector2 size = MeasureTextEx(game.font, text[i], fontSize, 0);

        rec[i] = (Rectangle){
            game.screenWidth / 4 - size.x / 2 - 10,
            game.screenHeight / 4 + 100 + i * fontSize - 5,
            size.x + 20,
            size.y + 10};

        DrawTextEx(
            game.font,
            text[i],
            (Vector2){
                game.screenWidth / 4 - size.x / 2,
                game.screenHeight / 4 + 100 + i * fontSize},
            fontSize,
            0,
            RED);
    }
}

int check_highscore(Game game)
{
    if (game.score <= 0)
        return 0;

    for (int i = 0; i < HIGHSCORE_MAX; i++)
    {
        if (game.score > game.high_score[i].score)
        {
            return 1;
        }
    }

    return 0;
}

void draw_new_high_menu(Game *game, float *timer, const char *text)
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("WOW! You got a new highscore!", game->screenWidth / 2 - MeasureText("WOW! You can a new highscore!", 40) / 2, game->screenHeight / 2 - 20, 40, RED);
    DrawText(TextFormat("SCORE %d", game->score), game->screenWidth / 2 - MeasureText(TextFormat("SCORE %d", game->score), 30) / 2, game->screenHeight / 2 + 25, 30, RED);
    DrawText("Enter your name: ", game->screenWidth / 2 - 250, game->screenHeight / 2 + 100, 30, RED);
    if (text == NULL)
        DrawText("", game->screenWidth / 2 - 245 + MeasureText("Enter your name: ", 30), game->screenHeight / 2 + 105, 20, WHITE);
    else
        DrawText(text, game->screenWidth / 2 - 245 + MeasureText("Enter your name: ", 30), game->screenHeight / 2 + 105, 20, WHITE);

    DrawRectangleLinesEx((Rectangle){game->screenWidth / 2 - 250 + MeasureText("Enter your name: ", 30), game->screenHeight / 2 + 100, 300, 30}, 2, WHITE);

    if ((*timer) >= 1.0f && (*timer) < 4.0f)
    {
        DrawText("Press Enter to Save", game->screenWidth / 2 - MeasureText("Press Enter to Save", 30) / 2, game->screenHeight / 2 + 150, 30, RED);

        if ((*timer) >= 2.0f)
            (*timer) = 0;
    }
    else
        DrawLineEx((Vector2){game->screenWidth / 2 - 250 + MeasureText("Enter your name: ", 30) + 10 + MeasureText(text, 20), game->screenHeight / 2 + 100 + 5}, (Vector2){game->screenWidth / 2 - 250 + MeasureText(text, 20) + MeasureText("Entry your name: ", 30) + 10, game->screenHeight / 2 + 100 + 25}, 2, WHITE);

    EndDrawing();
}

bool select_menu(Rectangle rec[], int *index, int count)
{
    Vector2 mouse = GetMousePosition();

    for (int i = 0; i < count; i++)
    {
        if (CheckCollisionPointRec(mouse, rec[i]))
        {
            *index = i;
            return true;
        }
    }

    return false;
}
