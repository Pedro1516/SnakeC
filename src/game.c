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
    if(game.score <= 0)
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
    if(text == NULL)
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

void read_keyboard(Text *text)
{
    int key = GetKeyPressed();

    if (key > 0)
    {
        if (text->character_inserted < text->limit)
        {
            if ((key >= 32 && key <= 93))
            {
                text->text[text->character_inserted] = key;
                text->character_inserted++;
            }
        }

        if (text->character_inserted > 0 && key == KEY_BACKSPACE)
        {
            text->character_inserted--;
            text->text[text->character_inserted] = 0;
        }
    }
}
