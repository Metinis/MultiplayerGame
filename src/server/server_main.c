#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#include "server.h"
#include "../network.h"
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int main(void) {

    ServerData server_data;
    server_init(&server_data);

    while (1) {

        server_update(&server_data);

        //usleep(100000);  // 100ms

        // usleep(16667);  // ~60 FPS
    }

    // After chatting close the socket
    socket_close(server_data.server_sock);

    return 0;
}
