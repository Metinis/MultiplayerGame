#pragma once
#include <arpa/inet.h>
#include <cglm/call/ivec2.h>

#include "types.h"

#define MAXLINE 1024
#define PORT 8080
#define SA struct sockaddr
#define MAX_PACKET_AGE 300
#define MAX_PLAYERS 10

void send_position_packet(int server_sock, const struct sockaddr_in *addr, const Player *player);

void socket_close(int sockfd);
