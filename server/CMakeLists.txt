cmake_minimum_required(VERSION 3.26)
project(server C)

set(CMAKE_C_STANDARD 11)

add_executable(server main.c server.c
        net/handle_client.c
        net/config.h
        net/handle_client.h)

if (WIN32)
    target_link_libraries(server ws2_32)
endif()
