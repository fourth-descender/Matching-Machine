cmake_minimum_required(VERSION 3.15)

project(m_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

file(GLOB SERVER_SOURCES "src/*.cpp")
list(REMOVE_ITEM SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp")
file(GLOB CLIENT_SOURCES "src/*.cpp")
list(REMOVE_ITEM CLIENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")


# for server.
add_executable(server ${SERVER_SOURCES})
target_include_directories(server PRIVATE include)

# for client.
add_executable(client ${CLIENT_SOURCES})
target_include_directories(client PRIVATE include)