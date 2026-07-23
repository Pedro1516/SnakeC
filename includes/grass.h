#include <raylib.h>
#ifndef GRASS_H
#define GRASS_H

typedef enum WindDirection
{
    WIND_LEFT,
    WIND_RIGHT,
    WIND_NONE
} WindDirection;

typedef struct GrassAnimation
{
    int frame_count;
    int animation_frame;
    float frame_duration;
    float animation_timer;
} GrassAnimation;

typedef struct Grass
{
    Texture2D *textures;
    WindDirection wind_direction;
    GrassAnimation animation;
    int texture_capacity;
} Grass;

Grass *init_grass(WindDirection wind_direction);
void load_texture_grass(Grass *grass, const char *path);
void update_grass(Grass *grass);
void UnloadGrass(Grass *grass);
void draw_grass(Grass *grass);

#endif