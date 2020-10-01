cmake_minimum_required(VERSION 3.17)
project(discord_isa)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(isabot main.cpp ArgData.cpp ArgData.h NetClient.cpp NetClient.h HTTPClient.cpp HTTPClient.h Ssl.cpp Ssl.h HttpResonse.cpp HttpResonse.h)

target_link_libraries(isabot ssl)