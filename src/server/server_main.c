#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#include "../network.h"
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int main(void) {

    ServerData server_data;
    server_init(&server_data);

    while (1) {

        server_update(&server_data);

        usleep(16667);  // ~60 FPS
    }

    // After chatting close the socket
    socket_close(&server_data.sockfd);

    return 0;
}
