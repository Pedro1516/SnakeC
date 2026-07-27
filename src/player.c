#include "../includes/player.h"
#include "../includes/game_state.h"
#include "../includes/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <raymath.h>

#define VISUAL_SMOOTH 0.2f // 0 = nunca alcança (curva infinita), 1 = sem atraso (sem suavização)

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
        free(p->texture);
        free(p);
        return NULL;
    }

    p->head->direction = (Vector2){1, 0};
    p->speed = 3;
    p->tail = NULL;

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
    h->visual_pos = (Vector2){head.x, head.y};
    h->prev_visual_pos = h->visual_pos;
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

void apply_curve_transition(Node *current)
{
    Vector2 target = *(Vector2 *)peek_queue(current->curve);
    Vector2 to_target = Vector2Subtract(target, (Vector2){current->rect.x, current->rect.y});

    if (Vector2DotProduct(to_target, current->direction) >= 0)
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

void draw_player(Player *player, int debbugmode)
{
    if (player == NULL)
        return;

    Node *node = player->head;
    if (node == NULL)
        return;

    Rectangle source, dest, shadow;
    Vector2 origin;
    int i = 0;
    float ang;

    for (int pass = 0; pass < 2; pass++)
    {
        for (Node *node = player->head; node != NULL; node = node->prox)
        {
            if (node->prox == NULL)
                i = 2;
            else if (node == player->head)
                i = 0;
            else
                i = 1;

            source = (Rectangle){0, 0, player->texture[i].width, player->texture[i].height};
            origin = (Vector2){player->texture[i].width / 2, player->texture[i].height / 2};

            dest.x = node->visual_pos.x + player->texture[i].width / 2;
            dest.y = node->visual_pos.y + player->texture[i].height / 2;
            dest.width = player->texture[i].width;
            dest.height = player->texture[i].height;

            shadow = dest;
            shadow.x += 3;
            shadow.y += 3;

            Vector2 delta = Vector2Subtract(node->prev_visual_pos, node->visual_pos); // <- ordem trocada
            if (Vector2LengthSqr(delta) > 0.0001f)
                ang = atan2f(delta.y, delta.x) * RAD2DEG - 90;
            else
                ang = atan2f(node->direction.y, node->direction.x) * RAD2DEG - 90;

            if (pass == 0)
                DrawTexturePro(player->texture[i], source, shadow, origin, ang, (Color){0, 0, 0, 127});
            else
            {
                DrawTexturePro(player->texture[i], source, dest, origin, ang, WHITE);
                if (debbugmode)
                    DrawRectangleLines(node->rect.x, node->rect.y, node->rect.width, node->rect.height, MAGENTA);
            }
        }
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

        // movimento REAL: sempre exato, sem lerp -> colisão e espaçamento continuam corretos
        current->rect.x -= player->speed * current->direction.x;
        current->rect.y -= player->speed * current->direction.y;

        if (current->curve->head != NULL && current->direction_curve->head != NULL)
        {
            apply_curve_transition(current);
        }

        // posição VISUAL: persegue o rect real com atraso -> curva suave só no desenho
        current->prev_visual_pos = current->visual_pos;
        current->visual_pos.x += (current->rect.x - current->visual_pos.x) * VISUAL_SMOOTH;
        current->visual_pos.y += (current->rect.y - current->visual_pos.y) * VISUAL_SMOOTH;

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