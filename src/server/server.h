#pragma once
#include <cglm/call/ivec2.h>
#include <sys/_types/_socklen_t.h>
#include "../network.h"
#include "../utils.h"
#include <fcntl.h>

typedef struct {
    int server_sock, client_sock;
    socklen_t len;
    struct sockaddr_in servaddr, cli;
    Player players[MAX_PLAYERS];
    int player_count;
} ServerData;

void server_init(ServerData *server_data);

void server_update(ServerData *server_data);
