cmake_minimum_required(VERSION 3.16)
project(bbtetris C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-lncurses")

add_executable(bbtetris main.c graphics.c graphics.h scene/title.c scene/title.h scene/marathon.c scene/marathon.h utils.h game.c game.h)