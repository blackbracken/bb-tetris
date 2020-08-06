#ifndef BBTETRIS_COMPONENTS_H
#define BBTETRIS_COMPONENTS_H

#include "../tetris/game.h"
#include "../record.h"

extern const char *TEXT_SUCCESS;
extern const char *TEXT_FAILURE;
extern const char *TEXT_NEW_RECORD;
extern const char *TEXT_RETURN;
extern const char *TEXT_ENTER_YOUR_NAME;

void repeat_routine_regularly(Board *board, bool (*routine)(Board *, int));

void draw_field(const Board *board, int y, int x);

void draw_hold(const Board *board, int y, int x);

void draw_next(Board *board, int y, int x);

void draw_rewards(Board *board, int y, int x);

void draw_mino(const Tetrimino *mino, int y, int x, int spin);

void draw_stats(Board *board, const char *mode_text, int y, int x);

void show_result_and_wait(
        GameResult result, char *record, Statistics *statistics, void (*recording)(char *, Statistics *));

void send_input(Board *sent, int input_key);

#endif
