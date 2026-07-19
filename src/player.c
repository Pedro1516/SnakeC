#include "../includes/player.h"
#include "../includes/game_state.h"

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
    Node *current = player->head;

    if (player == NULL)
        return;

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


void scoreup(Player *player, Game *game){
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