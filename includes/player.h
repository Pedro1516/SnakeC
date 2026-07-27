#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "types.h"

typedef struct node
{
    Rectangle rect;
    Vector2 direction;
    Vector2 visual_pos;       // posição usada só pro desenho (suavizada)
    Vector2 prev_visual_pos;  // pra calcular o ângulo pela tangente do movimento visual
    Queue *curve;
    Queue *direction_curve;
    struct node *prox;
} Node;

 struct Player
{
    Node *head;
    Node *tail;
    Texture *texture;
    float speed;
    int health;
    int damage;
} ;

void draw_player(Player *player, int debbugmode);
Player *create_player(Rectangle head);
Node *create_head(Rectangle head);
Vector2 calc_pos_node(Player *player);
void add_node(Player *player);
void move_player(Player *player);
void register_curve(Player *player);
int check_gameover(Player *player, Game *game);
void scoreup(Player *player, Game *game);
void kill_player(Player *player);
Vector2 *create_vector2(float x, float y);

#endif