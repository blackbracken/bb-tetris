#include "marathon.h"

#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "../graphics.h"
#include "../tetris/game.h"
#include "result_marathon.h"

const int FPS = 30;
const int DELAY_MILLI_PER_FRAME = 1000 / FPS;

const char *TEXT_HOLD = "HOLD";
const char *TEXT_NEXT = "NEXT";

void draw_board_frame(int field_orig_y, int field_orig_x);

void draw_mino(const Tetrimino *mino, int y, int x, int spin);

void erase_background_of_field(int orig_y, int orig_x);

void start_marathon(int lines) {
    int field_orig_x = WINDOW_WIDTH / 2 - 12;
    int field_orig_y = 10;

    { // prepare the view
        timeout(DELAY_MILLI_PER_FRAME);
        erase();
        draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);
        draw_board_frame(field_orig_y, field_orig_x);
        erase_background_of_field(field_orig_y, field_orig_x);
    }

    Board board;
    make_board(&board);

    int frame = 1;
    struct timespec delta_timespec;
    while (true) {
        timespec_get(&delta_timespec, TIME_UTC);
        long milli_start = delta_timespec.tv_nsec / 1000 / 1000;

        bool is_buried = !render(&board, frame, FPS);

        { // draw view
            erase_background_of_field(field_orig_y, field_orig_x);

            // draw a board
            for (int j = 0; j < FIELD_HEIGHT; j++) {
                for (int i = 0; i < FIELD_WIDTH; i++) {
                    if (board.field[j][i] != AIR) {
                        attrset(COLOR_PAIR(to_color_id(board.field[j][i])));
                        mvaddstr(field_orig_y + j, field_orig_x + 2 * i, "  ");
                    }
                }
            }

            draw_mino(
                    board.dropping_mino,
                    field_orig_y + board.dropping_mino_y,
                    field_orig_x + 2 * board.dropping_mino_x,
                    board.dropping_mino_spin
            );

            // draw held mino
            if (board.held_mino != NULL) {
                int y = field_orig_y + 3;
                int x = field_orig_x - 2 - 6;

                attrset(COLOR_PAIR(COLOR_ID_NONE));

                for (int j = -2; j < 6; j++) {
                    for (int i = -2; i < 6; i++) {
                        mvaddch(y + j, x + i, ' ');
                    }
                }
                draw_mino(board.held_mino, y, x, 0);
            }

            // draw next minos
            for (int next_idx = 0; next_idx < NEXT_AMOUNT; next_idx++) {
                int y = field_orig_y + 3 + 4 * next_idx;
                int x = field_orig_x + 2 * FIELD_WIDTH + 2 + 4;

                attrset(COLOR_PAIR(COLOR_ID_NONE));

                for (int j = -2; j < 6; j++) {
                    for (int i = -2; i < 6; i++) {
                        mvaddch(y + j, x + i, ' ');
                    }
                }
                draw_mino(peek_next(&board, next_idx), y, x, 0);
            }

            refresh();
        }

        if (is_buried || board.statistics.total_removed_lines >= lines) {
            disp_marathon_result(lines, &board.statistics);
            return;
        }

        int input_key = getch();
        switch (input_key) {
            case 'a':
            case KEY_LEFT:
                try_move_left(&board);
                break;
            case 'd':
            case KEY_RIGHT:
                try_move_right(&board);
                break;
            case 'w':
            case KEY_UP:
                drop_hardly(&board);
                break;
            case 's':
            case KEY_DOWN:
                drop_softly(&board);
                break;
            case 'k':
                try_spin_right(&board);
                break;
            case 'j':
                try_spin_left(&board);
                break;
            case ' ':
                try_hold(&board);
                break;
            default:
                break;
        }

        if (++frame % (FPS + 1) == 0) frame = 1;

        timespec_get(&delta_timespec, TIME_UTC);
        long milli_finish = delta_timespec.tv_nsec / 1000 / 1000;
        if (milli_finish < milli_start) milli_finish += 1000;

        // stabilize fps
        long milli_delta = DELAY_MILLI_PER_FRAME - (milli_finish - milli_start);
        if (input_key != ERR && milli_delta > 0) {
            usleep(milli_delta * 1000);
        }
    }
}

void draw_board_frame(int field_orig_y, int field_orig_x) {

    attrset(COLOR_PAIR(COLOR_ID_FIELD));
    {
        // draw field walls
        for (int i = 1; i < FIELD_HEIGHT; i++) {
            mvaddstr(field_orig_y + i, field_orig_x - 2, "[]");
            mvaddstr(field_orig_y + i, field_orig_x + 2 * FIELD_WIDTH, "[]");
        }

        // draw field bottom
        for (int i = -2; i < 2 * FIELD_WIDTH + 2; i++) {
            mvaddch(field_orig_y + FIELD_HEIGHT, field_orig_x + i, 'T');
        }
    }

    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    {
        mvaddstr(field_orig_y, field_orig_x - (2 + 2) - strlen(TEXT_HOLD), TEXT_HOLD);
        mvaddstr(field_orig_y, field_orig_x + 2 * FIELD_WIDTH + 2 + 4, TEXT_NEXT);
    }
}

void erase_background_of_field(int orig_y, int orig_x) {
    attrset(COLOR_PAIR(COLOR_ID_FIELD));

    for (int j = 0; j < FIELD_HEIGHT; j++) {
        for (int i = 0; i < FIELD_WIDTH; i++) {
            bool should_be_period = 2 <= j && i % 2 == 0 && j % 2 == 0;

            mvaddch(orig_y + j, orig_x + i, should_be_period ? '.' : ' ');
            mvaddch(orig_y + j, orig_x + i + 10, should_be_period ? '.' : ' ');
        }
    }
}

void draw_mino(const Tetrimino *mino, int y, int x, int spin) {
    attrset(COLOR_PAIR(to_color_id(mino->color)));

    for (int j = 0; j < mino->size; j++) {
        for (int i = 0; i < mino->size; i++) {
            if (mino->shape[spin][j][i]) {
                mvaddstr(y + (j - mino->center_y), x + 2 * (i - mino->center_x), "  ");
            }
        }
    }
}