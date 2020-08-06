#include "components.h"

#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../graphics.h"
#include "../utils.h"

const char *TEXT_SUCCESS = "SUCCESS!";
const char *TEXT_FAILURE = "FAILURE...";
const char *TEXT_NEW_RECORD = "NEW RECORD!";
const char *TEXT_RETURN = "PRESS SPACE KEY TO RETURN MENU";
const char *TEXT_ENTER_YOUR_NAME = "ENTER YOUR NAME AND PRESS SPACE KEY";

const char *TEXT_HOLD = "HOLD";
const char *TEXT_NEXT = "NEXT";

void repeat_routine_regularly(Board *board, bool (*routine)(Board *, int)) {
    timeout(DELAY_MILLI_PER_FRAME);

    int frame = 1;
    struct timespec delta_timespec;
    while (true) {
        timespec_get(&delta_timespec, TIME_UTC);
        long milli_start = delta_timespec.tv_nsec / 1000 / 1000;

        if (!routine(board, frame)) return;

        int input_key = getch();
        send_input(board, input_key);

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

void draw_rewards(Board *board, int y, int x) {
    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    for (int i = 0; i < 2 * FIELD_WIDTH + 4; i++) {
        mvaddch(y, x + i, ' ');
    }

    if (board->ren_count > 10) {
        attrset(COLOR_PAIR(COLOR_ID_HIGHEST));
    } else if (board->ren_count > 7) {
        attrset(COLOR_PAIR(COLOR_ID_HIGHER));
    } else {
        attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    }

    if (board->ren_count > 1) {
        mvprintw(y, x, "%d REN", board->ren_count);
    }

    if (board->did_back_to_back) {
        attrset(COLOR_PAIR(COLOR_ID_FIELD));
        mvprintw(y, x + 7, "BtB");
    }

    switch (board->removing_reward) {
        case TETRIS:
            attrset(COLOR_PAIR(COLOR_ID_FIELD));
            mvprintw(y, x + 11, "Tetris");
            break;
        case TSPIN_TRIPLE:
            attrset(COLOR_PAIR(COLOR_ID_HIGHER));
            mvprintw(y, x + 11, "T-spin Tri.");
            break;
        case TSPIN_DOUBLE:
            attrset(COLOR_PAIR(COLOR_ID_HIGH));
            mvprintw(y, x + 11, "T-spin Dou.");
            break;
        case TSPIN_SINGLE:
            attrset(COLOR_PAIR(COLOR_ID_PLAIN));
            mvprintw(y, x + 11, "T-spin Sin.");
            break;
        case PERFECT_CLEAR:
            attrset(COLOR_PAIR(COLOR_ID_HIGHEST));
            mvprintw(y, x + 11, "PERFECT CLEAR");
            break;
        default:
            break;
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

void draw_stats(Board *board, const char *mode_text, int y, int x) {
    draw_window_frame(y, x, y + 11, x + 23);
    attrset(COLOR_PAIR(COLOR_ID_PLAIN));

    mvprintw(y + 2, x + 3, "* MODE : %s", mode_text);
    mvprintw(y + 4, x + 3, "* LINES: %d", board->statistics.total_removed_lines);
    mvprintw(y + 6, x + 3, "* TIME : %02d:%02d",
             min(board->statistics.elapsed_seconds / 60, 99), board->statistics.elapsed_seconds % 60);
    mvprintw(y + 8, x + 3, "* SCORE: %d", board->statistics.score);
}

void show_result_and_wait(
        GameResult result,
        char *record,
        Statistics *statistics,
        void (*recording)(char *, Statistics *)) {
    int start_x = WINDOW_WIDTH / 4;
    int start_y = WINDOW_HEIGHT / 3;
    int end_x = WINDOW_WIDTH / 4 * 3;
    int end_y = WINDOW_HEIGHT / 3 * 2;
    int center_x = end_x / 2 + start_x / 2;

    draw_window_frame(start_y - 1, start_x - 2, end_y + 1, end_x + 2);
    attrset(COLOR_PAIR(COLOR_ID_NONE));
    for (int j = start_y; j < end_y; j++) {
        for (int i = start_x; i < end_x; i++) {
            mvaddch(j, i, ' ');
        }
    }

    const char *result_text;
    switch (result) {
        case RES_NEW_RECORD:
            attrset(COLOR_PAIR(COLOR_ID_SUCCESS));
            result_text = TEXT_NEW_RECORD;
            break;
        case RES_SUCCESS:
            attrset(COLOR_PAIR(COLOR_ID_SUCCESS));
            result_text = TEXT_SUCCESS;
            break;
        case RES_FAILURE:
            attrset(COLOR_PAIR(COLOR_ID_FAILURE));
            result_text = TEXT_FAILURE;
            break;
        default:
            result_text = "unreachable";
            break;
    }
    mvaddstr(start_y + 1, center_x - strlen(result_text) / 2, result_text);

    char record_text[64];
    sprintf(record_text, "record: %s", record);
    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    mvaddstr(start_y + 3, center_x - strlen(record_text) / 2, record_text);

    if (result == RES_NEW_RECORD) {
        mvaddstr(end_y - 2, center_x - strlen(TEXT_ENTER_YOUR_NAME) / 2, TEXT_ENTER_YOUR_NAME);
    } else {
        mvaddstr(end_y - 2, center_x - strlen(TEXT_RETURN) / 2, TEXT_RETURN);
    }

    refresh();

    if (result != RES_NEW_RECORD) {
        while (getch() != ' ');
    } else {
        char name[LENGTH_OF_NAME + 1];
        strcpy(name, "");
        int name_x = center_x - 5;
        int name_y = end_y - 6;

        mvprintw(name_y, name_x, "NAME: ____");

        while (true) {
            int input = getch();
            if (input == ' ' && 0 < strlen(name)) {
                break;
            }

            if (input == KEY_BACKSPACE && 0 < strlen(name)) {
                name[strlen(name) - 1] = '\0';
            }

            if (('A' <= input && input <= 'Z' || 'a' <= input && input <= 'z') && LENGTH_OF_NAME >= strlen(name) + 1) {
                sprintf(name, "%s%c", name, input);
            }

            mvprintw(name_y, name_x, "NAME: ");
            for (int i = 0; i < LENGTH_OF_NAME; i++) {
                mvaddch(name_y, name_x + 6 + i, i < strlen(name) ? name[i] : '_');
            }
        }

        recording(name, statistics);
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