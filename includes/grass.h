#ifndef GRASS_H
#define GRASS_H

#include <raylib.h>
#include "game_state.h"

typedef enum WindDirection
{
    WIND_LEFT,
    WIND_RIGHT,
    WIND_NONE
} WindDirection;

typedef struct Grass
{
    Texture2D *textures;
    WindDirection wind_direction;
    Animation animation;
    int texture_capacity;
} Grass;

Grass *init_grass(WindDirection wind_direction);
void load_texture_grass(Grass *grass, const char *path);
void update_grass(Grass *grass);
void UnloadGrass(Grass *grass);
void draw_grass(Grass *grass);

#endif