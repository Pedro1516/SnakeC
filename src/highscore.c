#include "../includes/highscore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *copy_data(const char *text_to_copy, char terminator, int initial_index)
{
    size_t text_len = strlen(text_to_copy);

    if ((size_t)initial_index >= text_len)
        return NULL;

    int len = 0;

    while (text_to_copy[initial_index + len] != '\0' && text_to_copy[initial_index + len] != terminator)
        len++;

    char *buffer = malloc(len + 1);
    if (buffer == NULL)
    {
        printf("Não foi possivel alocar memoria para o buffer.");
        return NULL;
    }

    memcpy(buffer, text_to_copy + initial_index, len);
    buffer[len] = '\0';
    return buffer;
}

HighScore *read_file()
{
    FILE *f = fopen("assets/data/highscores.txt", "r");
    if (f == NULL)
    {
        printf("Não foi possivel abrir o arquivo de highscore");
        fclose(f);
        return NULL;
    }

    HighScore *highscore = malloc(sizeof(HighScore) * HIGHSCORE_MAX);

    char buffer_geral[1000];
    int index_highscore = 0;

    while (index_highscore < HIGHSCORE_MAX && fgets(buffer_geral, sizeof(buffer_geral), f) != NULL)
    {
        char *buffer_name = copy_data(buffer_geral, (char)1, 0);
        char *buffer_score = copy_data(buffer_geral, '\n', strlen(buffer_name) + 1);

        if (buffer_name != NULL && buffer_score != NULL)
        {
            highscore[index_highscore].name = buffer_name;
            highscore[index_highscore].score = atoi(buffer_score);
            free(buffer_score);
            index_highscore++;
        }
    }

    fclose(f);
    return highscore;
}

int load_highscore(HighScore *highscore_to_load, HighScore *highscore_game)
{
    int i = 0;
    for (; i < HIGHSCORE_MAX; i++)
    {
        if (highscore_to_load[i].name == NULL)
            break;
        highscore_game[i] = highscore_to_load[i];
    }

    return i;
}

int save_highscore(HighScore *highscore)
{
    FILE *f = fopen("assets/data/highscores.txt", "w");
    if (f == NULL)
    {
        printf("Não foi possivel abrir o arquivo para o salvamento do Highscore.\n");
        fclose(f);
        return 1;
    }

    for (size_t i = 0; i < HIGHSCORE_MAX; i++)
    {
        if (highscore[i].score > 0)
        {
            fprintf(f, "%s", highscore[i].name);
            fprintf(f, "%c", (char)1);
            fprintf(f, "%d", highscore[i].score);
             fprintf(f, "\n");
        }
    }

    fclose(f);

    return 0;
}