#include "../includes/player.h"
#include <math.h>

Player *create_player(Color color, Rectangle head)
{
    Player *p = malloc(sizeof(Player));
    p->color = color;
    p->head = create_head(head);
    p->head->direction = (Vector2){1, 0};
    p->speed = 3;
    add_node(p);

    return p;
}

Node *create_head(Rectangle head)
{
    Node *h = malloc(sizeof(Node));
    h->prox = NULL;
    h->direction = (Vector2){0, 0};
    h->direction_curve = (Vector2){0, 0};
    h->curve = (Vector2){0, 0};
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

void move_player(Player *player)
{
    Vector2 pos_anterior = {0, 0};
    Vector2 curve = {0, 0};
    Node *current = player->head;

    if (player == NULL)
        return;

    DrawText(TextFormat("Current: %f, %f", current->prox->curve.x, current->prox->curve.y), 10, 10, 20, WHITE);
    while (true)
    {
        if (current == NULL)
            return;

        if (fabs(current->rect.x - current->curve.x) < 1 && fabs(current->rect.y - current->curve.y) < 1)
        {
            current->direction = current->direction_curve;

            if (current->prox != NULL)
            {
                current->prox->curve = current->curve;
                current->prox->direction_curve = current->direction_curve;
            }

            current->curve = (Vector2){0, 0};
            current->direction_curve = (Vector2){0, 0};
        }

        current->rect.x -= player->speed * current->direction.x;
        current->rect.y -= player->speed * current->direction.y;
        current = current->prox;
    }
}

void register_curve(Player *player)
{
    player->head->prox->curve = (Vector2){player->head->rect.x, player->head->rect.y};
    player->head->prox->direction_curve = player->head->direction;
}