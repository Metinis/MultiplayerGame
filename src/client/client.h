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

void client_send_handshake(int sock, struct sockaddr_in *server_addr);
void client_init(ClientData *client_data);
void client_process_position(ClientData *client_data, vec2 position, uint16_t player_id);
void client_process_handshake(ClientData *client_data, char *message, uint16_t player_id, const Player* players, uint16_t player_count);
void client_receive_packet(ClientData *client_data);