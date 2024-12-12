#include "client.h"

#include <string.h>

extern void client_send_handshake(const int sock, struct sockaddr_in *server_addr) {
    const char *handshake_message = "Hello, Server!";

    Packet packet;
    packet.packet_type = HANDSHAKE;
    packet.timestamp = get_current_time_in_ms();
    strcpy(packet.data.message, handshake_message);

    const ssize_t sent_bytes = sendto(sock, &packet, sizeof(packet), 0,
                                (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (sent_bytes == -1) {
        perror("Failed to send handshake message");
    } else {
        printf("Handshake message sent to server\n");
    }
}
extern void client_init(ClientData *client_data) {
    client_data->server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_data->server_sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    memset(&client_data->servaddr, 0, sizeof(client_data->servaddr));
    client_data->servaddr.sin_family = AF_INET;
    client_data->servaddr.sin_addr.s_addr = INADDR_ANY;
    client_data->servaddr.sin_port = htons(PORT);
    fcntl(client_data->server_sock, F_SETFL, O_NONBLOCK);

    client_send_handshake(client_data->server_sock, &client_data->servaddr);

    //receive id
    client_receive_packet(client_data);
}
extern void client_process_handshake(ClientData *client_data, char *message, const uint16_t player_id, const Player* players, const uint16_t player_count) {
    //printf("\nReceived message: %s\n", message);
    //for(size_t i = 0; i < 1; i++) {
    //    client_data->players[i] = players[i];
    //}
    //client_data->player_count = player_count;

    //client_data->player_id = player_id;
}
extern void client_process_position(ClientData *client_data, vec2 position, const uint16_t player_id) {
    //printf("\nprocessing position %fx, %fy, id %d", position[0], position[1], player_id);
    memcpy(client_data->players[player_id].position, position, sizeof(vec2));
}
extern void client_receive_packet(ClientData *client_data) {
    struct sockaddr_in addr;
    Packet packet;
    socklen_t addr_len = sizeof(addr);

    const ssize_t bytes_received = recvfrom(client_data->server_sock, &packet, sizeof(packet), 0,
                                      (struct sockaddr *)&addr, &addr_len);

    if(bytes_received <= 0) {
        return;
    }

    switch(packet.packet_type) {
        case HANDSHAKE:
            client_process_handshake(client_data, packet.data.message, packet.player_id, packet.data.players, packet.data.player_count);
        break;
        case POSITION:
            client_process_position(client_data, packet.data.position, packet.player_id);
        break;
        case MESSAGE:
            printf("Received message: %s\n", packet.data.message);
        break;
        case NEW_CONNECTION:
            printf("New player connected! ID: %d\n", packet.player_id);
            client_data->players[packet.player_id].addr = addr;
            client_data->players[packet.player_id].player_id = packet.player_id;
            client_process_position(client_data, packet.data.position, packet.player_id);
            client_data->player_count++;
        break;
        default:
            printf("Unknown packet type received: %d\n", packet.packet_type);
        break;
    }
}
