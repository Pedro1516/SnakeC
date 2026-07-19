#include "../includes/game_state.h"
#include "../includes/player.h"

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
    const char *text[3];
    int measure_text[3];
    int font_size = 40;

    text[0] = "Play";
    measure_text[0] = MeasureText(text[0], font_size);

    text[1] = "Highscore";
    measure_text[1] = MeasureText(text[1], font_size);

    text[2] = "Settings";
    measure_text[2] = MeasureText(text[2], font_size);

    for (int i = 0; i < 3; i++)
    {
        if (i == index_menu)
            DrawRectangle(game.screenWidth / 2 - measure_text[i] / 2 - 5, game.screenHeight / 2 - (font_size + 10) / 2 + 100 + 60 * i, measure_text[i] + 10, font_size + 10, (Color){255, 255, 255, 127});

        DrawText(text[i], game.screenWidth / 2 - measure_text[i] / 2, game.screenHeight / 2 - (font_size + 10) / 2 + 100 + 60 * i, font_size, RED);
    }
}

int check_highscore(Game game)
{
    for (int i = 0; i < HIGHSCORE_MAX; i++)
    {
        if (game.score > game.high_score[i].score)
        {
            return 1;
        }
    }

    return 0;
}