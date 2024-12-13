#pragma once
#include <fcntl.h>
#include <sys/_types/_socklen_t.h>
#include "../network.h"
#include "../utils.h"

typedef struct {
    int server_sock, connfd;
    struct sockaddr_in servaddr, cli;
    uint16_t player_id;
    Player players[MAX_PLAYERS];
    int player_count;
} ClientData;

void client_init(ClientData *client_data);

void client_receive_packet(ClientData *client_data);
