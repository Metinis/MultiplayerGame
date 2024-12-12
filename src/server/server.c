#include "server.h"

#include <string.h>
#include <secure/_string.h>

extern void server_process_handshake(ServerData *server_data, char *message, struct sockaddr_in *addr) {
    printf("\nReceived message: %s\n", message);
    if(is_new_player(server_data, addr)) {
        add_new_player(server_data, addr);
    }
}

extern void server_process_position(ServerData *server_data, vec2 position, const struct sockaddr_in *addr) {
    for (int i = 0; i < server_data->player_count; i++) {
        Player *player = &server_data->players[i];

        if (memcmp(&player->addr, addr, sizeof(struct sockaddr_in)) == 0) {
            memcpy(player->position, position, sizeof(vec2));
            return;
        }
    }
    printf("No player found for the address, possible unregistered player\n");
}

extern void server_receive_packet(ServerData *server_data) {
    struct sockaddr_in addr;
    Packet packet;
    socklen_t addr_len = sizeof(addr);

    const ssize_t bytes_received = recvfrom(server_data->server_sock, &packet, sizeof(packet), 0,
                                      (struct sockaddr *)&addr, &addr_len);


    if(bytes_received <= 0) {
        return;
    }

    switch(packet.packet_type) {
        case HANDSHAKE:
            server_process_handshake(server_data, packet.data.message, &addr);
            break;
        case POSITION:

            server_process_position(server_data, packet.data.position, &addr);
        //new position packet received, update server state
            for (size_t i = 0; i < server_data->player_count; i++) {

            const Player *current_player = &server_data->players[i];
            //send this players data to other players, including itself just in case, we will do the check in game loop anyways not to render servers pos
            for(size_t j = 0; j < server_data->player_count; j++) {
                const Player *receiving_player = &server_data->players[j];
                //printf("\nplayer[%lu]", j);
                send_position_packet(server_data->server_sock, &receiving_player->addr, current_player);
            }

        }
            break;
        case MESSAGE:
            printf("Received message: %s\n", packet.data.message);
            break;
        default:
            printf("Unknown packet type received: %d\n", packet.packet_type);
        break;
    }
}
extern uint8_t is_new_player(const ServerData *server_data, const struct sockaddr_in *cli_addr) {
    for (int i = 0; i < server_data->player_count; i++) {
        if (memcmp(&server_data->players[i].addr, cli_addr, sizeof(struct sockaddr_in)) == 0) {
            return 0;
        }
    }
    return 1;
}
extern void server_send_new_connection(const int sock, struct sockaddr_in *addr, const uint16_t player_id, vec2 pos) {
    Packet packet;
    packet.packet_type = NEW_CONNECTION;
    packet.player_id = player_id;
    packet.timestamp = get_current_time_in_ms();
    memcpy(packet.data.position, pos, sizeof(vec2));

    sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *)addr, sizeof(*addr));
}
extern void add_new_player(ServerData *server_data, struct sockaddr_in *cli_addr) {
    if (server_data->player_count >= MAX_PLAYERS) {
        printf("Maximum number of players reached\n");
        return;
    }
    Player *new_player = &server_data->players[server_data->player_count];
    new_player->player_id = server_data->player_count;
    new_player->addr = *cli_addr;
    new_player->position[0] = 100;
    new_player->position[1] = 100;
    server_data->player_count++;
    printf("New player added! Total players: %d\n", server_data->player_count);

    //send handshake to assign player id on client side
    server_send_handshake(server_data, cli_addr, new_player->player_id);
    //let clients know of new connection
    for(size_t j = 0; j < server_data->player_count; j++) {
        Player *receiving_player = &server_data->players[j];
        if(memcmp(&receiving_player->addr, cli_addr, sizeof(struct sockaddr_in)) == 0) {
            break;
        }
        server_send_new_connection(server_data->server_sock, &receiving_player->addr, new_player->player_id, new_player->position);
    }
}
extern void server_init(ServerData *server_data) {
    server_data->server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_data->server_sock == -1) {
        printf("\nFailed to create socket");
    }
    else {
        printf("\nSocket successfully created");
    }
    // assign IP, PORT
    memset(&server_data->servaddr, 0, sizeof(server_data->servaddr));
    memset(&server_data->cli, 0, sizeof(server_data->cli));
    server_data->servaddr.sin_family = AF_INET;
    server_data->servaddr.sin_addr.s_addr = INADDR_ANY;
    server_data->servaddr.sin_port = htons(PORT);
    fcntl(server_data->server_sock, F_SETFL, O_NONBLOCK);
    server_data->player_count = 0;

    if ( bind(server_data->server_sock, (const struct sockaddr *)&server_data->servaddr,
            sizeof(server_data->servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}
extern void server_update(ServerData *server_data) {
    server_receive_packet(server_data);
}
extern void server_send_handshake(ServerData *server_data, struct sockaddr_in *addr, const uint16_t player_id) {
    const char *handshake_message = "Hello, Client!";

    Packet packet;
    packet.packet_type = HANDSHAKE;
    packet.player_id = player_id;
    packet.timestamp = get_current_time_in_ms();
    packet.data.players = server_data->players;
    //packet.data.player_count = server_data->player_count;
    //strcpy(packet.data.message, handshake_message);

    const ssize_t sent_bytes = sendto(server_data->server_sock, &packet, sizeof(packet), 0,
                                (struct sockaddr *)addr, sizeof(*addr));
    if (sent_bytes == -1) {
        perror("Failed to send handshake message");
    } else {
        printf("Handshake message sent to client\n");
    }
}