#include "network.h"

#include <unistd.h>

extern void client_init(ClientData *client_data) {
    // socket create and verification
    client_data->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_data->sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&client_data->servaddr, sizeof(client_data->servaddr));

    // assign IP, PORT
    client_data->servaddr.sin_family = AF_INET;
    client_data->servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_data->servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(client_data->sockfd, (SA*)&client_data->servaddr, sizeof(client_data->servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
        }
    else
        printf("connected to the server..\n");
}
extern void client_receive_position(Position *pos, const int *connfd) {
    // Receive the position data from the server
    int n = recv(*connfd, pos, sizeof(Position), 0);
    if (n <= 0) {
        perror("Failed to receive position data");
    }
}
extern void client_send_position(Position *pos, const int *connfd) {
    send(*connfd, &pos, sizeof(Position), 0);
}
extern void server_init(ServerData *server_data) {
    server_data->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_data->sockfd == -1) {
        printf("\n Failed to create socket");
    }
    else {
        printf("\n Socket successfully created");
    }
    bzero(&server_data->servaddr, sizeof(server_data->servaddr));

    // assign IP, PORT
    server_data->servaddr.sin_family = AF_INET;
    server_data->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_data->servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(server_data->sockfd, (SA*)&server_data->servaddr, sizeof(server_data->servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(server_data->sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    server_data->len = sizeof(server_data->cli);

    // Accept the data packet from client and verification
    server_data->connfd = accept(server_data->sockfd, (SA*)&server_data->cli, &server_data->len);
    if (server_data->connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    server_data->player_pos.x = 0;
    server_data->player_pos.y = 0;
}
extern void server_update(ServerData *server_data) {
    int n = recv(server_data->sockfd, &server_data->player_pos, sizeof(Position), 0);
    if (n <= 0) {
        perror("Failed to receive position data");
    }
    // Send the player position to the client
    send(server_data->connfd, &server_data->player_pos, sizeof(Position), 0);
}
extern void socket_close(const int *sockfd) {
    close(*sockfd);
}