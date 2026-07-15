#include "../includes/player.h"

Player *create_player(Color color, Rectangle head)
{
    Player *p = malloc(sizeof(Player));
    p->color = color;
    p->head = create_head(head);
    p->direction = (Vector2){-1, 0};
    p->speed = 180;
    add_node(p, (Vector2){head.x, head.y + head.height});

    return p;
}

Node *create_head(Rectangle head)
{
    Node *h = malloc(sizeof(Node));
    h->prox = NULL;
    h->rect = head;

    return h;
}

void add_node(Player *player, Vector2 pos)
{
    if (player == NULL)
        return;

    Node *h = player->head;
    while (true)
    {
        if (h->prox == NULL)
        {
            h->prox = create_head((Rectangle){pos.x, pos.y, h->rect.width, h->rect.height});
            break;
        }
        else
        {
            h = h->prox;
        }
    }
}

void draw_player(Player *player)
{
    if (player == NULL)
        return;

    Node *node = player->head;
    if (node == NULL)
        return;

    while (true)
    {
        DrawRectangleRec(node->rect, player->color);
        node = node->prox;
        if (node == NULL)
            break;
    }
}

void move_player(Player *player, float deltatime)
{
    if (player == NULL)
        return;

    Node *atual = player->head;
    if (atual == NULL)
        return;

    atual->rect.x += player->speed * player->direction.x * deltatime;   //Move a cabeça
    atual->rect.y += player->speed * player->direction.y * deltatime;
    atual = atual->prox;

    while (true)
    {

        atual = atual->prox;
        if (atual == NULL)
            return;
    }
}