#include <raylib.h>
#include <stdlib.h>

typedef struct node
{
    Rectangle rect;
    Vector2 direction;
    Vector2 curve;
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
void add_node(Player *player);
void move_player(Player *player, float deltatime);
Vector2 calc_pos_node(Player *player);