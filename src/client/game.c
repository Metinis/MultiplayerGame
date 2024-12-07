#include "game.h"

#include <SDL.h>

extern void game_init(Game *game) {
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

    game->player_pos.x = 100;
    game->player_pos.y = 100;

    game->quit = 0;

    game->last_time = SDL_GetTicks();
    game->delta_time = 0.0f;
}
extern void game_run(Game *game) {
        // Calculate delta time
        const Uint32 current_time = SDL_GetTicks();
        game->delta_time = (current_time - game->last_time) / 1000.0f; // Convert to seconds
        game->last_time = current_time;


        // Receive the updated player position from the server
        //receive_position(&player_pos, sockfd);

        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);  // Set color to black
        SDL_RenderClear(game->renderer);  // Clear the screen with the set color

        //Draw red rectangle
        SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);  // Set color to red
        const SDL_Rect rect = { game->player_pos.x, game->player_pos.y, 400, 300 };
        SDL_RenderFillRect(game->renderer, &rect);

        SDL_RenderPresent(game->renderer);

        while (SDL_PollEvent(&game->e)) {
            switch(game->e.type) {
                case SDL_QUIT:
                    game->quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (game->e.key.keysym.scancode) {
                        case SDL_SCANCODE_D:
                            game->player_pos.x += 100.0f * game->delta_time * 10.0f;
                            break;
                    }
            }
        }

        //send_position(&player_pos, sockfd);

        // Add a short delay to control the frame rate
        SDL_Delay(16);  // Roughly 60 FPS
}
extern void game_close(const Game *game) {

    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

