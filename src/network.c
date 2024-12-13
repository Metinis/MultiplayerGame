#include "network.h"

#include <unistd.h>
#include <string.h>

#include "utils.h"



void send_position_packet(const int server_sock, const struct sockaddr_in *addr, const Player *player) { //player is data we are sending to addr
    Packet packet;
    packet.packet_type = POSITION;
    packet.player_id = player->player_id;
    packet.timestamp = get_current_time_in_ms();
    memcpy(packet.data.position, player->position, sizeof(vec2));

    sendto(server_sock, &packet, sizeof(packet), 0, (struct sockaddr *)addr, sizeof(*addr));
}

void socket_close(const int sockfd) {
    close(sockfd);
}