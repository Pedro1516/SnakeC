#include "../includes/grass.h"
#include <stdlib.h>
#include <raylib.h>

Grass *init_grass(WindDirection wind_direction)
{
    Grass *grass = malloc(sizeof(Grass));
    if (grass == NULL)
        return NULL;

    grass->textures = malloc(sizeof(Texture) * 5);
    if (grass->textures == NULL)
    {
        free(grass);
        return NULL;
    }

    grass->texture_capacity = 5;
    grass->wind_direction = wind_direction;
    grass->animation.animation_frame = 0;
    grass->animation.animation_timer = 0;
    grass->animation.frame_count = 0;
    grass->animation.frame_duration = 0.25f;

    return grass;
}

void load_texture_grass(Grass *grass, const char *path)
{
    if (grass->animation.frame_count == grass->texture_capacity)
    {
        grass->texture_capacity *= 2;
        grass->textures = realloc(grass->textures, sizeof(Texture) * grass->texture_capacity);
    }

    grass->textures[grass->animation.frame_count] = LoadTexture(path);
    grass->animation.frame_count++;
}

void UnloadGrass(Grass *grass)
{
    for (int i = 0; i < grass->animation.frame_count; i++)
    {
        UnloadTexture(grass->textures[i]);
    }

    free(grass->textures);
    free(grass);
}

void update_grass(Grass *grass)
{
    grass->animation.animation_timer += GetFrameTime();

    if (grass->animation.animation_timer >= grass->animation.frame_duration)
    {
        grass->animation.animation_timer = 0;

        grass->animation.animation_frame++;

        if (grass->animation.animation_frame >= grass->animation.frame_count)
            grass->animation.animation_frame = 0;
    }
}

void draw_grass(Grass *grass)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    static Vector2 list_pos[100];
    static bool first_exec = true;

    if (first_exec)
    {
        for (size_t i = 0; i < 100; i++)
            list_pos[i] = (Vector2){GetRandomValue(0, screenWidth), GetRandomValue(0, screenHeight)};
        first_exec = false;
    }

    for (size_t i = 0; i < 100; i++)
    {
        DrawTexture(grass->textures[grass->animation.animation_frame], list_pos[i].x, list_pos[i].y, WHITE);
    }
}
