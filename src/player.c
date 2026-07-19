#include "../includes/player.h"
#include "../includes/game_state.h"
#include "../includes/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <raymath.h>

Player *create_player(Color color, Rectangle head)
{
    Player *p = malloc(sizeof(Player));
    if (p == NULL)
        return NULL;

    p->color = color;
    p->head = create_head(head);
    if (p->head == NULL)
    {
        free(p);
        return NULL;
    }

    p->head->direction = (Vector2){1, 0};
    p->speed = 3;
    add_node(p);

    return p;
}

Vector2 *create_vector2(float x, float y)
{
    Vector2 *vector = malloc(sizeof(Vector2));
    if (vector == NULL)
        return NULL;

    *vector = (Vector2){x, y};
    return vector;
}

Node *create_head(Rectangle head)
{
    Node *h = malloc(sizeof(Node));
    if (h == NULL)
        return NULL;

    h->prox = NULL;
    h->direction = (Vector2){0, 0};
    h->direction_curve = create_queue();
    if (h->direction_curve == NULL)
        return NULL;
    h->curve = create_queue();
    if (h->curve == NULL)
        return NULL;
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
            Node *aux = create_head((Rectangle){pos.x, pos.y, h->rect.width, h->rect.height});
            if (aux == NULL)
                return;

            h->prox = aux;
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
    if (player == NULL)
        return;

    Node *current = player->head;

    while (true)
    {
        if (current == NULL)
            return;

        current->rect.x -= player->speed * current->direction.x;
        current->rect.y -= player->speed * current->direction.y;

        if (current->curve->head != NULL && current->direction_curve->head != NULL)
        { // fabs(current->rect.x - (*(Vector2 *)peek_queue(current->curve)).x) < 1 && fabs(current->rect.y - (*(Vector2 *)peek_queue(current->curve)).y) < 1
            if (Vector2Distance(*(Vector2 *)peek_queue(current->curve), (Vector2){current->rect.x, current->rect.y}) <= 1)
            {
                current->direction = *(Vector2 *)peek_queue(current->direction_curve);

                Vector2 *curve = dequeue(current->curve);
                Vector2 *curve_direction = dequeue(current->direction_curve);

                if (current->prox != NULL)
                {
                    enqueue(current->prox->curve, create_vector2(curve->x, curve->y));
                    enqueue(current->prox->direction_curve, create_vector2(curve_direction->x, curve_direction->y));
                }

                free(curve);
                free(curve_direction);
            }
        }

        current = current->prox;
    }
}

void register_curve(Player *player)
{
    if (!enqueue(player->head->prox->curve, create_vector2(player->head->rect.x, player->head->rect.y)))
        printf("Problema ao enfileirar curva\n");

    if (!enqueue(player->head->prox->direction_curve, create_vector2(player->head->direction.x, player->head->direction.y)))
        printf("Problema ao enfileirar direção da curva\n");
}

int check_gameover(Player *player, Game *game)
{
    if (player->head->rect.x + player->head->rect.width > game->screenWidth || player->head->rect.x < 0 || player->head->rect.y + player->head->rect.height > game->screenHeight || player->head->rect.y < 0)
    {
        game->state = GAME_GAMEOVER;
        return 1;
    }
    // if(player->head->prox != NULL)
    // {
    //     Node *aux = player->head->prox;
    //     while (true)
    //     {
    //         if (CheckCollisionRecs(player->head->rect, aux->rect))
    //         {
    //             game->state = GAME_GAMEOVER;
    //             break;
    //         }
    //         if (aux->prox == NULL)
    //             break;
    //         aux = aux->prox;
    //     }
    // }

    return 0;
}

void scoreup(Player *player, Game *game)
{
    add_node(player);
    game->score++;
}

void kill_player(Player *player)
{
    if (player == NULL)
        return;

    Node *current = player->head;

    while (current != NULL)
    {
        Node *next = current->prox;
        free(current);
        current = next;
    }

    free(player);
}