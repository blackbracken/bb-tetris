#ifndef BBTETRIS_COMPONENTS_H
#define BBTETRIS_COMPONENTS_H

#include "../tetris/game.h"

extern const char *TEXT_SUCCESS;
extern const char *TEXT_FAILURE;
extern const char *TEXT_RETURN;

void start_routine(Board *board, bool (*routine)(Board *, int));

void draw_field(const Board *board, int y, int x);

void draw_hold(const Board *board, int y, int x);

void draw_next(Board *board, int y, int x);

void draw_rewards(Board *board, int y, int x);

void draw_mino(const Tetrimino *mino, int y, int x, int spin);

void send_input(Board *sent, int input_key);

#endif
