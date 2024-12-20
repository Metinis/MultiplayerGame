#include "game.h"
#include "../network.h"


int main()
{
    ClientData client_data;
    client_init(&client_data);

    Game game;
    game_init(&game, client_data.player_id);

    while (!game.quit) {
        game_run(&game, &client_data);
    }

    game_close(&game);

    socket_close(client_data.server_sock);
}
