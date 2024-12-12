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

void server_send_handshake(ServerData *server_data, struct sockaddr_in *addr, uint16_t player_id); //used to send clients id back
void server_send_new_connection(int sock, struct sockaddr_in *addr, uint16_t player_id, vec2 pos);
void server_receive_packet(ServerData *server_data);
void server_init(ServerData *server_data);
void server_update(ServerData *server_data);
uint8_t is_new_player(const ServerData *server_data, const struct sockaddr_in *cli_addr);
void add_new_player(ServerData *server_data, struct sockaddr_in *cli_addr);
void server_process_position(ServerData *server_data, vec2 position, const struct sockaddr_in *addr);
void server_process_handshake(ServerData *server_data, char *message, struct sockaddr_in *addr);