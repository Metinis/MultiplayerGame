#include "game.h"

#include <SDL.h>

extern void game_init(Game *game, const uint16_t player_id) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
    }

    // Create a window
    game->window = SDL_CreateWindow("Multiplayer Game",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          1280, 720, SDL_WINDOW_SHOWN);
    if (game->window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
    }

    game->player.player_id = player_id;

    game->player.position[0] = 100;
    game->player.position[1] = 100;

    game->quit = 0;

    game->last_time = SDL_GetTicks();
    game->delta_time = 0.0f;
}
extern void game_run(Game *game, ClientData *client_data) {
    const uint32_t frame_start = SDL_GetTicks();

    game_update_delta_time(game);

    client_receive_packet(client_data);

    game_render(game, game->renderer, client_data->players, client_data->player_count, client_data->player_id);

    const uint8_t moved = game_poll_events(game);

    if(moved) {
        send_position_packet(client_data->server_sock, &client_data->servaddr, &game->player);
    }


    const uint32_t frame_time = SDL_GetTicks() - frame_start;
    if (frame_time < 16) {
        SDL_Delay(16 - frame_time);
    }
}
extern void game_render(const Game *game, SDL_Renderer *renderer, const Player *players, const size_t players_size, const uint16_t player_id) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    const SDL_Rect rect = { game->player.position[0], game->player.position[1], 100, 100 };
    SDL_RenderFillRect(renderer, &rect);

    for(size_t i = 0; i < players_size; i++) {
        if(i != player_id) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            const SDL_Rect server_rect = { players[i].position[0], players[i].position[1], 100, 100 };
            SDL_RenderFillRect(renderer, &server_rect);
        }
    }

    SDL_RenderPresent(renderer);
}
extern void game_update_delta_time(Game *game) {
    const Uint32 current_time = SDL_GetTicks();
    game->delta_time = (current_time - game->last_time) / 1000.0f; // Convert to seconds
    game->last_time = current_time;
}
extern uint8_t game_poll_events(Game *game) {
    uint8_t moved = 0;
    while (SDL_PollEvent(&game->e)) {
        switch(game->e.type) {

            case SDL_QUIT:
                game->quit = 1;
            break;
        }
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_W]) {
            moved = 1;
            game->player.position[1] -= 100.0f * game->delta_time * 10;
        }
        if (state[SDL_SCANCODE_S]) {
            moved = 1;
            game->player.position[1] += 100.0f * game->delta_time * 10;
        }
        if (state[SDL_SCANCODE_D]) {
            moved = 1;
            game->player.position[0] += 100.0f * game->delta_time * 10;
        }
        if (state[SDL_SCANCODE_A]) {
            moved = 1;
            game->player.position[0] -= 100.0f * game->delta_time * 10;
        }
    }
    return moved;
}
extern void game_close(const Game *game) {

    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

