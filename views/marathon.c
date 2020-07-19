#include "marathon.h"

#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#include "../graphics.h"
#include "../tetris/game.h"

const int FPS = 30;
const int DELAY_MILLI_PER_FRAME = 1000 / FPS;

void draw_board_frame(int field_orig_y, int field_orig_x);

void erase_background_of_field(int orig_y, int orig_x);

void start_marathon(int lines) {
    int field_orig_x = WINDOW_WIDTH / 2 - 12;
    int field_orig_y = 10;

    { // prepare view
        efface_window();
        draw_window_frame();
        draw_board_frame(field_orig_y, field_orig_x);
        erase_background_of_field(field_orig_y, field_orig_x);
    }

    Board board;
    gen_board(&board);

    int frame = 1;
    struct timespec timespec;
    while (true) {
        timespec_get(&timespec, TIME_UTC);
        long milli_start = timespec.tv_nsec / 1000 / 1000;

        timeout(DELAY_MILLI_PER_FRAME);

        Result result = render(&board, frame, FPS);
        if (result.is_available) {
            return;
        }

        { // draw view
            erase_background_of_field(field_orig_y, field_orig_x);

            // draw a board
            for (int j = 0; j < FIELD_HEIGHT; j++) {
                for (int i = 0; i < FIELD_WIDTH; i++) {
                    if (board.field[j][i] != AIR) {
                        move(field_orig_y + j, field_orig_x + 2 * i);
                        attrset(COLOR_PAIR(to_color_id(board.field[j][i])));
                        addstr("  ");
                    }
                }
            }

            // draw a dropping mino
            for (int j = 0; j < board.dropping_mino->size; j++) {
                for (int i = 0; i < board.dropping_mino->size; i++) {
                    if (board.dropping_mino->shape[board.dropping_mino_spin][j][i]) {
                        move(
                                field_orig_y + board.dropping_mino_y + (j - board.dropping_mino->center_y),
                                field_orig_x + 2 * (board.dropping_mino_x + (i - board.dropping_mino->center_x))
                        );
                        attrset(COLOR_PAIR(to_color_id(board.dropping_mino->color)));
                        addstr("  ");
                    }
                }
            }

            refresh();
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
            default:
                break;
        }

        if (++frame % (FPS + 1) == 0) frame = 1;

        timespec_get(&timespec, TIME_UTC);
        long milli_finish = timespec.tv_nsec / 1000 / 1000;
        if (milli_finish < milli_start) milli_finish += 1000;

        // stabilize fps
        long milli_delta = DELAY_MILLI_PER_FRAME - (milli_finish - milli_start);
        if (input_key != ERR && milli_delta > 0) {
            usleep(milli_delta * 1000);
        }
    }
}

void draw_board_frame(int field_orig_y, int field_orig_x) {
    // draw walls
    for (int i = 1; i < FIELD_HEIGHT; i++) {
        move(field_orig_y + i, field_orig_x - 2);
        attrset(COLOR_PAIR(COLOR_ID_FIELD));
        addstr("[]");

        move(field_orig_y + i, field_orig_x + 2 * FIELD_WIDTH);
        attrset(COLOR_PAIR(COLOR_ID_FIELD));
        addstr("[]");
    }

    // draw bottom
    for (int i = -2; i < 2 * FIELD_WIDTH + 2; i++) {
        move(field_orig_y + FIELD_HEIGHT, field_orig_x + i);
        attrset(COLOR_PAIR(COLOR_ID_FIELD));
        addch('T');
    }
}

void erase_background_of_field(int orig_y, int orig_x) {
    for (int j = 1; j < FIELD_HEIGHT; j++) {
        for (int i = 0; i < FIELD_WIDTH; i++) {
            bool should_be_period = i % 2 == 0 && j % 2 == 0;

            move(orig_y + j, orig_x + i);
            attrset(COLOR_PAIR(COLOR_ID_FIELD));
            addch(should_be_period ? '.' : ' ');

            move(orig_y + j, orig_x + i + 10);
            attrset(COLOR_PAIR(COLOR_ID_FIELD));
            addch(should_be_period ? '.' : ' ');
        }
    }
}