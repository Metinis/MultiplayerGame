#include "game.h"
#include "../network.h"


int main()
{
    ClientData client_data;
    client_init(&client_data);

    Game game;
    game_init(&game);

    while (!game.quit) {
        game_run(&game);
    }

    game_close(&game);

    socket_close(&client_data.sockfd);
}
