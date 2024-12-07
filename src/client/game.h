#pragma once
#include <SDL_events.h>
#include <SDL_render.h>

#define MAX_PLAYERS 10

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Position player_pos;
    Position other_player_positions[MAX_PLAYERS];
    SDL_Event e;
    int quit;
    Uint32 last_time;
    float delta_time;
} Game;

void game_init(Game *game);

void game_run(Game *game);

void game_close(const Game *game);

