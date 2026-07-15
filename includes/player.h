#include <raylib.h>
#include <stdlib.h>

typedef struct node
{
    Rectangle rect;
    struct node *prox;
} Node;

typedef struct
{
    Node *head;
    Vector2 direction;
    Color color;
    float speed;
} Player;

void draw_player(Player *player);
Player *create_player(Color color, Rectangle head);
Node *create_head(Rectangle head);
void add_node(Player *player, Vector2 pos);
void move_player(Player *player, float deltatime);