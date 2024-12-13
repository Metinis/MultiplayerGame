#pragma once
#include <SDL_events.h>
#include <SDL_render.h>
#include "client.h"
#include "../types.h"
#include <cglm/cglm.h>
#include "../utils.h"

#define MAX_PLAYERS 10

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event e;
    int quit;
    Uint32 last_time;
    float delta_time;
    Player player;
} Game;

void game_init(Game *game, uint16_t player_id);

void game_run(Game *game, ClientData *client_data);

void game_close(const Game *game);

