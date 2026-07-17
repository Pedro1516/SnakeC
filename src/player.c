#include "../includes/player.h"
#include <math.h>

Player *create_player(Color color, Rectangle head)
{
    Player *p = malloc(sizeof(Player));
    p->color = color;
    p->head = create_head(head);
    p->head->direction = (Vector2){1, 0};
    p->speed = 180;
    add_node(p);

    return p;
}

Node *create_head(Rectangle head)
{
    Node *h = malloc(sizeof(Node));
    h->prox = NULL;
    h->direction = (Vector2){0, 0};
    h->rect = head;

    return h;
}

Vector2 calc_pos_node(Player *player)
{
    Node *aux = player->head;
    Vector2 pos;

    while (true)
    {
        if (aux->prox == NULL)
        {
            pos.x = aux->rect.x + aux->rect.width * aux->direction.x;
            pos.y = aux->rect.y + aux->rect.height * aux->direction.y;
            return pos;
        }

        aux = aux->prox;
    }
}

void add_node(Player *player)
{
    if (player == NULL)
        return;

    Node *h = player->head;
    Vector2 pos = calc_pos_node(player);
    while (true)
    {
        if (h->prox == NULL)
        {
            h->prox = create_head((Rectangle){pos.x, pos.y, h->rect.width, h->rect.height});
            h->prox->direction = h->direction;
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
    Vector2 pos_anterior = {0, 0};
    Vector2 curve = {0, 0};
    Node *current = player->head;

    if (player == NULL)
        return;

    while (true)
    {
        if (current == NULL)
            return;

        if (current == player->head)
        {
            pos_anterior = (Vector2){current->rect.x, current->rect.y};
            current->rect.x += player->speed * player->head->direction.x * deltatime; // Move a cabeça
            current->rect.y += player->speed * player->head->direction.y * deltatime;
        }
        else
        {
            Vector2 aux = {current->rect.x, current->rect.y};
            current->rect.x = pos_anterior.x;
            current->rect.y = pos_anterior.y;

            pos_anterior.x = aux.x + ((player->head->direction.x != 0) ? current->rect.width : 0);
            pos_anterior.y = aux.y + ((player->head->direction.y != 0) ? current->rect.height : 0);
        }

        current = current->prox;
    }
}