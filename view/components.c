#include "components.h"

#include <curses.h>
#include <string.h>
#include "../graphics.h"

const char *TEXT_HOLD = "HOLD";
const char *TEXT_NEXT = "NEXT";

void draw_field(const Board *board, int y, int x) {
    attrset(COLOR_PAIR(COLOR_ID_FIELD));
    for (int i = 1; i < FIELD_HEIGHT; i++) {
        mvaddstr(y + i, x - 2, "[]");
        mvaddstr(y + i, x + 2 * FIELD_WIDTH, "[]");
    }
    for (int i = -2; i < 2 * FIELD_WIDTH + 2; i++) {
        mvaddch(y + FIELD_HEIGHT, x + i, 'T');
    }

    for (int j = 0; j < FIELD_HEIGHT; j++) {
        for (int i = 0; i < FIELD_WIDTH; i++) {
            bool should_be_period = 2 <= j && i % 2 == 0 && j % 2 == 0;

            mvaddch(y + j, x + i, should_be_period ? '.' : ' ');
            mvaddch(y + j, x + i + 10, should_be_period ? '.' : ' ');
        }
    }
    for (int j = 0; j < FIELD_HEIGHT; j++) {
        for (int i = 0; i < FIELD_WIDTH; i++) {
            if (board->field[j][i] != AIR) {
                attrset(COLOR_PAIR(to_color_id(board->field[j][i])));
                mvaddstr(y + j, x + 2 * i, "  ");
            }
        }
    }

    draw_mino(
            board->dropping_mino,
            y + board->dropping_mino_y,
            x + 2 * board->dropping_mino_x,
            board->dropping_mino_spin
    );
}

void draw_hold(const Board *board, int y, int x) {
    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    mvaddstr(y, x, TEXT_HOLD);

    if (board->held_mino != NULL) {
        attrset(COLOR_PAIR(COLOR_ID_NONE));

        for (int j = -2; j < 6; j++) {
            for (int i = -2; i < 6; i++) {
                mvaddch(y + j + 3, x + i, ' ');
            }
        }

        draw_mino(board->held_mino, y + 3, x, 0);
    }
}

void draw_next(Board *board, int y, int x) {
    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    mvaddstr(y, x, TEXT_NEXT);

    for (int next_idx = 0; next_idx < NEXT_AMOUNT; next_idx++) {
        int next_x = x;
        int next_y = y + 3 + 4 * next_idx;

        attrset(COLOR_PAIR(COLOR_ID_NONE));

        for (int j = -2; j < 6; j++) {
            for (int i = -2; i < 6; i++) {
                mvaddch(
                        next_y + j,
                        next_x + i,
                        ' '
                );
            }
        }

        draw_mino(peek_next(board, next_idx), next_y, next_x, 0);
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

void send_input(Board *sent, int input_key) {
    switch (input_key) {
        case 'a':
        case KEY_LEFT:
            try_move_left(sent);
            break;
        case 'd':
        case KEY_RIGHT:
            try_move_right(sent);
            break;
        case 'w':
        case KEY_UP:
            drop_hardly(sent);
            break;
        case 's':
        case KEY_DOWN:
            drop_softly(sent);
            break;
        case 'k':
            try_spin_right(sent);
            break;
        case 'j':
            try_spin_left(sent);
            break;
        case ' ':
            try_hold(sent);
            break;
        default:
            break;
    }
}