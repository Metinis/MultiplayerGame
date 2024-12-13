cmake_minimum_required(VERSION 3.10)
project(MultiplayerGame)

# Set the source files for both the server and client
set(SERVER_SOURCES
        src/server/server_main.c
        src/network.c
        src/network.h
        src/utils.c
        src/utils.h
        src/types.c
        src/types.h
        src/server/server.c
        src/server/server.h
)

set(CLIENT_SOURCES
        src/client/client_main.c
        src/network.c
        src/client/game.c
        src/client/game.h
        src/network.h
        src/utils.c
        src/utils.h
        src/types.c
        src/types.h
        src/client/client.c
        src/client/client.h
)

# Find SDL2 package
find_package(SDL2 REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Create an executable for the server
add_executable(server ${SERVER_SOURCES})

# Create an executable for the client
add_executable(client ${CLIENT_SOURCES})

# Link SDL2 with the executables
target_link_libraries(server ${SDL2_LIBRARIES})
target_link_libraries(client ${SDL2_LIBRARIES})

add_subdirectory(include/cglm)
include_directories(include/cglm/include)

target_link_libraries(server cglm)
target_link_libraries(client cglm)

# Include header files directory
include_directories(include)
