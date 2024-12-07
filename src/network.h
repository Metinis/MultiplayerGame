#pragma once
#include "client/game.h"
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

typedef struct {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
} ClientData;

typedef struct {
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli;
    Position player_pos;
} ServerData;

void client_init(ClientData *client_data);
void client_receive_position(Position *pos, const int *connfd);
void client_send_position(Position *pos, const int *connfd);
void server_init(ServerData *server_data);
void server_update(ServerData *server_data);
void socket_close(const int *sockfd);
