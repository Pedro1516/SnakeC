
#include <raylib.h>
#include <stdlib.h>
#ifndef PLAYER_H
#define PLAYER_H

typedef struct node
{
    Rectangle rect;
    Vector2 direction;
    Vector2 curve;
    Vector2 direction_curve;
    struct node *prox;
} Node;

typedef struct
{
    Node *head;
    Color color;
    float speed;
} Player;

void draw_player(Player *player);
Player *create_player(Color color, Rectangle head);
Node *create_head(Rectangle head);
Vector2 calc_pos_node(Player *player);
void add_node(Player *player);
void move_player(Player *player);
void register_curve(Player *player);
#endif