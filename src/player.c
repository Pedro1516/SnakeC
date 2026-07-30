#include "../includes/player.h"
#include "../includes/game_state.h"
#include "../includes/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <raymath.h>

#define VISUAL_SMOOTH 0.2f // 0 = nunca alcança (curva infinita), 1 = sem atraso (sem suavização)

Snake create_snake(Rectangle head)
{
    Snake snake = {0};
    snake.head = create_head(head);
    if (snake.head == NULL)
        return (Snake){0};

    snake.head->direction = (Vector2){1, 0};
    snake.speed = 3;
    snake.tail = NULL;
    add_node(&snake);

    return snake;
}

CombatStats create_stats()
{
    CombatStats stats = {0};
    stats.acuracy = 90;
    stats.attack = 1;
    stats.defense = 1;
    stats.health = 10;
    stats.max_health = 10;
    stats.lucky = 1;

    return stats;
}

Player *create_player(Rectangle head)
{
    Player *p = calloc(1, sizeof(Player));
    if (p == NULL)
    {
        return NULL;
    }

    int texture_count = 4;
    bool invalid_texture = false;
    p->texture = malloc(sizeof(Texture) * texture_count);
    p->texture[0] = LoadTexture("assets/textures/snake/snake0.png");
    p->texture[1] = LoadTexture("assets/textures/snake/snake1.png");
    p->texture[2] = LoadTexture("assets/textures/snake/snake2.png");
    p->texture[3] = LoadTexture("assets/textures/snake/snake_battle.png");

    for (int i = 0; i < texture_count; i++)
        if (p->texture[i].id == 0)
            invalid_texture = true;

    p->snake = create_snake(head);
    p->status = create_stats();

    if (p->snake.head == NULL || invalid_texture)
    {
        for (int i = 0; i < texture_count; i++)
        {
            if (p->texture[i].id != 0)
                UnloadTexture(p->texture[i]);
        }

        free(p->texture);
        free(p);
        return NULL;
    }

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

Vector2 calc_pos_node(Snake *snake)
{
    Vector2 pos;

    if (snake == NULL)
        return (Vector2){-1, -1};

    if (snake->tail != NULL)
    {
        pos.x = snake->tail->rect.x + snake->tail->rect.width * snake->tail->direction.x;
        pos.y = snake->tail->rect.y + snake->tail->rect.height * snake->tail->direction.y;
        return pos;
    }
    else
    {
        pos.x = snake->head->rect.x + snake->head->rect.width * snake->head->direction.x;
        pos.y = snake->head->rect.y + snake->head->rect.height * snake->head->direction.y;
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

void add_node(Snake *snake)
{
    if (snake == NULL)
        return;

    Vector2 pos = calc_pos_node(snake);
    Node *node = create_head((Rectangle){pos.x, pos.y, 30, 30});
    if (node == NULL)
        return;

    if (snake->tail == NULL)
    {
        snake->head->prox = node;
        node->direction = snake->head->direction;
        snake->tail = node;

        return;
    }

    snake->tail->prox = node;
    snake->tail->prox->direction = snake->tail->direction;
    snake->tail = snake->tail->prox;
}

void draw_player(Player *player, int debbugmode)
{
    if (player == NULL)
        return;

    Node *node = player->snake.head;
    if (node == NULL)
        return;

    Rectangle source, dest, shadow;
    Vector2 origin;
    int i = 0;
    float ang;

    for (int pass = 0; pass < 2; pass++)
    {
        for (Node *node = player->snake.head; node != NULL; node = node->prox)
        {
            if (node->prox == NULL)
                i = 2;
            else if (node == player->snake.head)
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

void move_player(Snake *snake)
{
    if (snake == NULL)
        return;

    Node *current = snake->head;

    while (true)
    {
        if (current == NULL)
            return;

        // movimento REAL: sempre exato, sem lerp -> colisão e espaçamento continuam corretos
        current->rect.x -= snake->speed * current->direction.x;
        current->rect.y -= snake->speed * current->direction.y;

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

void register_curve(Snake *snake)
{
    if (!enqueue(snake->head->prox->curve, create_vector2(snake->head->rect.x, snake->head->rect.y)))
        printf("Problema ao enfileirar curva\n");

    if (!enqueue(snake->head->prox->direction_curve, create_vector2(snake->head->direction.x, snake->head->direction.y)))
        printf("Problema ao enfileirar direção da curva\n");
}

int check_gameover(Snake *snake, Game *game)
{
    if (snake->head->rect.x + snake->head->rect.width > game->screenWidth || snake->head->rect.x < 0 || snake->head->rect.y + snake->head->rect.height > game->screenHeight || snake->head->rect.y < 0)
    {
        game->state = GAME_GAMEOVER;
        return 1;
    }
    if (snake->head->prox != NULL)
    {
        Node *aux = snake->head->prox;
        while (true)
        {
            if (CheckCollisionRecs(snake->head->rect, aux->rect) && snake->head->prox != aux)
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

void scoreup(Snake *snake, Game *game)
{
    add_node(snake);
    game->score++;
}

void kill_player(Player *player)
{
    if (player == NULL)
        return;

    Node *current = player->snake.head;

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