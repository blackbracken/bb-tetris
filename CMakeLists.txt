cmake_minimum_required(VERSION 3.16)
project(bbtetris C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-lncurses")

add_executable(bbtetris main.c graphics.c graphics.h views/title.c views/title.h views/marathon.c views/marathon.h utils.h tetris/game.c tetris/game.h tetris/tetrimino.c tetris/tetrimino.h views/result_marathon.c views/result_marathon.h)