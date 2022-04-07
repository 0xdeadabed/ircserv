cmake_minimum_required(VERSION 3.20)
project(ircserv)

set(CMAKE_CXX_STANDARD 98)

file(GLOB SRCS src/*.cpp)
add_executable(ircserv ${SRCS})

add_subdirectory(tests)
