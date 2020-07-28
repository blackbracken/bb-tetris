cmake_minimum_required(VERSION 3.16)
project(bbtetris C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-lncurses")

add_executable(bbtetris main.c graphics.c graphics.h view/title.c view/title.h view/marathon.c view/marathon.h utils.h tetris/game.c tetris/game.h tetris/tetrimino.c tetris/tetrimino.h view/components.c view/components.h view/ultra.c view/ultra.h view/40line.c view/40line.h)