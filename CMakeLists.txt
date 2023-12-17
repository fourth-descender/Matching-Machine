cmake_minimum_required(VERSION 3.15)

project(m_Engine)

find_program(GPP_COMPILER g++)
if(GPP_COMPILER)
    set(CMAKE_CXX_COMPILER ${GPP_COMPILER})
    message("compiler found: ${CMAKE_CXX_COMPILER}")
else()
    message("g++ compiler not found, using default system compiler")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
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