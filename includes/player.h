#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "types.h"
#include "combat_stats.h"

typedef struct node
{
    Rectangle rect;
    Vector2 direction;
    Vector2 visual_pos;      // posição usada só pro desenho (suavizada)
    Vector2 prev_visual_pos; // pra calcular o ângulo pela tangente do movimento visual
    Queue *curve;
    Queue *direction_curve;
    struct node *prox;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
    float move_speed;
    int speed;
} Snake;

struct Player
{
    Snake snake;
    CombatStats status;
    Texture *texture;
};

struct Item
{
    const char *name;
    int amount;
    int type;
    Texture *texture;
};

void draw_player(Player *player, int debbugmode);
Player *create_player(Rectangle head);
Node *create_head(Rectangle head);
Vector2 calc_pos_node(Snake *snake);
void add_node(Snake *snake);
void move_player(Snake *snake);
void register_curve(Snake *snake);
int check_gameover(Snake *snake, Game *game);
void scoreup(Snake *snake, Game *game);
void kill_player(Player *player);
Vector2 *create_vector2(float x, float y);

#endif