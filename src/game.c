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
    (*player) = create_player(WHITE, (Rectangle){game->screenWidth / 2 - 15, game->screenWidth / 2 - 15, 30, 30});
    game->score = 0;
}

char *string_duplicate(const char *str)
{
    char *copy = malloc(strlen(str) + 1);

    if (copy != NULL)
        strcpy(copy, str);

    return copy;
}

void draw_menu(Game game, int index_menu)
{

    const char *text[4];
    Vector2 measure_text[4];
    int font_size = 60;

    text[0] = "Play";
    measure_text[0] = MeasureTextEx(game.font, text[0], font_size, 0);

    text[1] = "Highscore";
    measure_text[1] = MeasureTextEx(game.font, text[1], font_size, 0);

    text[2] = "Settings";
    measure_text[2] = MeasureTextEx(game.font, text[2], font_size, 0);

    text[3] = "Quit";
    measure_text[3] = MeasureTextEx(game.font, text[3], font_size, 0);

    for (int i = 0; i < 4; i++)
    {
        if (i == index_menu)
            DrawRectangle(game.screenWidth / 2 - measure_text[i].x / 2 - 5, game.screenHeight / 2 - ( measure_text[i].y + 10) / 2 + 100 + 60 * i,  measure_text[i].x + 10,  measure_text[i].y + 10, (Color){255, 255, 255, 127});
            // DrawText(text[i], game.screenWidth / 2 - measure_text[i] / 2, game.screenHeight / 2 - (font_size + 10) / 2 + 100 + 60 * i, font_size, RED);
        Vector2 pos = {game.screenWidth / 2 - measure_text[i].x / 2, game.screenHeight / 2 - ( measure_text[i].y + 10) / 2 + 100 + 60 * i};
        DrawTextEx(game.font, text[i], pos, font_size, 0, RED);
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
    DrawText("WOW! You can a new highscore!", game->screenWidth / 2 - MeasureText("WOW! You can a new highscore!", 40) / 2, game->screenHeight / 2 - 20, 40, RED);
    DrawText(TextFormat("SCORE %d", game->score), game->screenWidth / 2 - MeasureText(TextFormat("SCORE %d", game->score), 30) / 2, game->screenHeight / 2 + 25, 30, RED);
    DrawText("Entry your name: ", game->screenWidth / 2 - 250, game->screenHeight / 2 + 100, 30, RED);
    if (text == NULL)
        DrawText("", game->screenWidth / 2 - 245 + MeasureText("Entry your name: ", 30), game->screenHeight / 2 + 105, 20, WHITE);
    else
        DrawText(text, game->screenWidth / 2 - 245 + MeasureText("Entry your name: ", 30), game->screenHeight / 2 + 105, 20, WHITE);

    DrawRectangleLinesEx((Rectangle){game->screenWidth / 2 - 250 + MeasureText("Entry your name: ", 30), game->screenHeight / 2 + 100, 300, 30}, 2, WHITE);

    if ((*timer) >= 1.0f && (*timer) < 4.0f)
    {
        DrawText("Press Enter to Save", game->screenWidth / 2 - MeasureText("Press Enter to Save", 30) / 2, game->screenHeight / 2 + 150, 30, RED);

        if ((*timer) >= 2.0f)
            (*timer) = 0;
    }
    else
        DrawLineEx((Vector2){game->screenWidth / 2 - 250 + MeasureText("Entry your name: ", 30) + 10 + MeasureText(text, 20), game->screenHeight / 2 + 100 + 5}, (Vector2){game->screenWidth / 2 - 250 + MeasureText(text, 20) + MeasureText("Entry your name: ", 30) + 10, game->screenHeight / 2 + 100 + 25}, 2, WHITE);

    EndDrawing();
}

