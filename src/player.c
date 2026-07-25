#include "../includes/player.h"
#include "../includes/game_state.h"
#include "../includes/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <raymath.h>

Player *create_player(Rectangle head)
{
    Player *p = malloc(sizeof(Player));
    if (p == NULL)
    {
        free(p);
        return NULL;
    }

    p->texture = malloc(sizeof(Texture) * 3);
    p->texture[0] = LoadTexture("assets/textures/snake/snake0.png");
    p->texture[1] = LoadTexture("assets/textures/snake/snake1.png");
    p->texture[2] = LoadTexture("assets/textures/snake/snake2.png");

    if (p->texture[0].id == 0 || p->texture[1].id == 0 || p->texture[2].id == 0)
    {
        free(p->texture);
        free(p);
        return NULL;
    }

    p->head = create_head(head);
    if (p->head == NULL)
    {
        free(p);
        free(p->texture);
        return NULL;
    }

    p->head->direction = (Vector2){1, 0};
    p->speed = 3;
    p->tail = NULL;
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
    Vector2 pos;

    if (player == NULL)
        return (Vector2){-1, -1};

    if (player->tail != NULL)
    {
        pos.x = player->tail->rect.x + player->tail->rect.width * player->tail->direction.x;
        pos.y = player->tail->rect.y + player->tail->rect.height * player->tail->direction.y;
        return pos;
    }
    else
    {
        pos.x = player->head->rect.x + player->head->rect.width * player->head->direction.x;
        pos.y = player->head->rect.y + player->head->rect.height * player->head->direction.y;
        return pos;
    }
}

void add_node(Player *player)
{
    if (player == NULL)
        return;

    Vector2 pos = calc_pos_node(player);
    Node *node = create_head((Rectangle){pos.x, pos.y, 30, 30});
    if (node == NULL)
        return;

    if (player->tail == NULL)
    {
        player->head->prox = node;
        node->direction = player->head->direction;
        player->tail = node;

        return;
    }

    player->tail->prox = node;
    player->tail->prox->direction = player->tail->direction;
    player->tail = player->tail->prox;
}

void draw_player(Player *player)
{
    if (player == NULL)
        return;

    Node *node = player->head;
    if (node == NULL)
        return;

    Rectangle source = {
        0, 0,
        player->texture[0].width,
        player->texture[0].height};

    Rectangle dest = {
        node->rect.x,
        node->rect.y,
        player->texture[0].width,
        player->texture[0].height};

    Vector2 origin = {
        player->texture[0].width / 2.0f,
        player->texture[0].height / 2.0f};

    float ang = -round(atan2(node->direction.x, node->direction.y) * (180 / PI));
    DrawTexturePro(player->texture[0], source, dest, origin, ang, WHITE);

    node = node->prox;
    if (node == NULL)
        return;

    while (true)
    {
        if (node->prox == NULL)
        {
            dest.x = node->rect.x;
            dest.y = node->rect.y;
            ang = -round(atan2(node->direction.x, node->direction.y) * (180 / PI));
            DrawTexturePro(player->texture[2], source, dest, origin, ang, WHITE);
        }
        else
        {
            dest.x = node->rect.x;
            dest.y = node->rect.y;
            ang = -round(atan2(node->direction.x, node->direction.y) * (180 / PI));
            DrawTexturePro(player->texture[1], source, dest, origin, ang, WHITE);
        }

        node = node->prox;
        if (node == NULL)
            break;
    }
}

void apply_curve_transition(Node *current)
{
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
        {
            apply_curve_transition(current);
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
    if (player->head->prox != NULL)
    {
        Node *aux = player->head->prox;
        while (true)
        {
            if (CheckCollisionRecs(player->head->rect, aux->rect) && player->head->prox != aux)
            {
                game->state = GAME_GAMEOVER;
                return 1;
            }
            if (aux->prox == NULL)
                break;
            aux = aux->prox;
        }
    }

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
        destroy_queue(current->curve);
        destroy_queue(current->direction_curve);
        free(current);
        current = next;
    }

    free(player);
}