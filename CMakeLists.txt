cmake_minimum_required(VERSION 3.17)
project(discord_isa)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(isabot main.cpp ArgData.cpp ArgData.h NetClient.cpp NetClient.h HttpClient.cpp HttpClient.h HttpResponse.cpp HttpResponse.h HttpHead.cpp HttpHead.h HttpChunk.cpp HttpChunk.h Json.cpp Json.h DiscordClient.cpp DiscordClient.h JsonException.cpp JsonException.h)
add_executable(isabot-test test.cpp HttpHead.h HttpHead.cpp Json.h Json.cpp JsonException.h JsonException.cpp)

target_link_libraries(isabot ssl crypto)