#pragma once
#include <cglm/call/ivec2.h>
#include <netinet/in.h>
#define MAXLINE 1024

typedef struct {
    struct sockaddr_in addr;
    uint16_t player_id;
    vec2 position;
} Player;

typedef enum {
    HANDSHAKE = 0x01,
    POSITION = 0x02,
    MESSAGE = 0x03,
    NEW_CONNECTION = 0x04,
} PacketType;

typedef struct {
    PacketType packet_type;
    uint64_t timestamp;
    uint16_t player_id;
    union {
        vec2 position;
        char message[MAXLINE];
        Player* players;
        uint16_t player_count;
    } data;
} Packet;
