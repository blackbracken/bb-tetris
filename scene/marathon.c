#include "marathon.h"

#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#include "../graphics.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 21

typedef enum {
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    PURPLE,
    AIR
} MinoBlock;

typedef struct {
    const int center_x, center_y;
    const int shape[4][4][4];
    const int size;
    const int spin_offsets[4][5][2];
} Tetrimino;

typedef struct {
    MinoBlock field[FIELD_HEIGHT][FIELD_WIDTH];
    Tetrimino *dropping_mino;
    int dropping_mino_x, dropping_mino_y;
    int dropping_mino_spin;
    int dropping_mass_per_second;
} Board;

const int FPS = 30;
const int DELAY_MILLI_PER_FRAME = 1000 / FPS;

// TODO: write spinning offsets
Tetrimino MINO_T = {
        .center_x =  1,
        .center_y =  1,
        .shape = {
                {
                        {0, 1, 0},
                        {1, 1, 1},
                        {0, 0, 0},
                },
                {
                        {0, 1, 0},
                        {0, 1, 1},
                        {0, 1, 0},
                },
                {
                        {0, 0, 0},
                        {1, 1, 1},
                        {0, 1, 0},
                },
                {
                        {0, 1, 0},
                        {1, 1, 0},
                        {0, 1, 0},
                },
        },
        .size = 3,
};

void draw_board_frame(int field_orig_y, int field_orig_x);

void erase_background_of_field(int orig_y, int orig_x);

int can_move(Board *board, void (*predicate)(Board *));

void move_left(Board *board);

void move_right(Board *board);

void drop_softly(Board *board);

void drop_hardly(Board *board);

void spin_left(Board *board);

void spin_right(Board *board);

int can_move(Board *board, void (*predicate)(Board *)) {
    Board assumed = *board;
    predicate(&assumed);

    for (int j = 0; j < assumed.dropping_mino->size; j++) {
        for (int i = 0; i < assumed.dropping_mino->size; i++) {
            if (assumed.dropping_mino->shape[assumed.dropping_mino_spin][j][i] == 0) {
                continue;
            }
            int dest_x = assumed.dropping_mino_x + (i - assumed.dropping_mino->center_x);
            int dest_y = assumed.dropping_mino_y + (j - assumed.dropping_mino->center_y);

            if (dest_x < 0 || dest_y < 0
                || FIELD_WIDTH <= dest_x || FIELD_HEIGHT <= dest_y
                || assumed.field[dest_y][dest_x] != 0) {
                return false;
            }
        }
    }

    return true;
}

void move_left(Board *board) { board->dropping_mino_x--; }

void move_right(Board *board) { board->dropping_mino_x++; }

void drop_softly(Board *board) {
    board->dropping_mino_y++;
}

void drop_hardly(Board *board) {
    while (can_move(board, drop_softly)) drop_softly(board);
}

void spin_right(Board *board) {
    board->dropping_mino_spin = (board->dropping_mino_spin + 1) % 4;
}

void spin_left(Board *board) {
    board->dropping_mino_spin = (board->dropping_mino_spin + 3) % 4;
}


_Noreturn void start_marathon(int lines) {
    int field_orig_x = WINDOW_WIDTH / 2 - 12;
    int field_orig_y = 10;
    { // render a view
        efface_window();
        draw_window_frame();
        draw_board_frame(field_orig_y, field_orig_x);
        erase_background_of_field(field_orig_y, field_orig_x);
    }

    Board board = {
            .field = {0},
            .dropping_mino = &MINO_T,
            .dropping_mino_x = 4,
            .dropping_mino_y = 2,
            .dropping_mino_spin = 0,
            .dropping_mass_per_second = 1,
    };

    int frame = 1;
    struct timespec timespec;
    while (1) {
        timespec_get(&timespec, TIME_UTC);
        long milli_start = timespec.tv_nsec / 1000 / 1000;

        timeout(DELAY_MILLI_PER_FRAME);

        int inputKey = getch();

        { // game routine
            if (frame % (FPS / board.dropping_mass_per_second) == 0) {
                // TODO: put mino
                if (can_move(&board, drop_softly)) drop_softly(&board);
            }

            erase_background_of_field(field_orig_y, field_orig_x);

            // render a board - unused yet
            for (int j = 0; j < FIELD_HEIGHT; j++) {
                for (int i = 0; i < FIELD_WIDTH; i++) {
                    if (board.field[j][i]) {
                        move(field_orig_y + j, field_orig_x + 2 * i);
                        attrset(COLOR_PAIR(COLOR_ID_MINO_T));
                        addstr("  ");
                    }
                }
            }

            // render a dropping mino
            for (int j = 0; j < board.dropping_mino->size; j++) {
                for (int i = 0; i < board.dropping_mino->size; i++) {
                    if (board.dropping_mino->shape[board.dropping_mino_spin][j][i]) {
                        move(
                                field_orig_y + board.dropping_mino_y + (j - board.dropping_mino->center_y),
                                field_orig_x + 2 * (board.dropping_mino_x + (i - board.dropping_mino->center_x))
                        );
                        attrset(COLOR_PAIR(COLOR_ID_MINO_T));
                        addstr("  ");
                    }
                }
            }
        }

        refresh();

        switch (inputKey) {
            case 'a':
            case KEY_LEFT:
                if (can_move(&board, move_left)) {
                    move_left(&board);
                }
                break;
            case 'd':
            case KEY_RIGHT:
                if (can_move(&board, move_right)) {
                    move_right(&board);
                }
                break;
            case 'w':
            case KEY_UP:
                drop_hardly(&board);
                break;
            case 's':
            case KEY_DOWN:
                if (can_move(&board, drop_softly)) {
                    drop_softly(&board);
                }
                break;
            case 'k':
                if (can_move(&board, spin_right)) {
                    spin_right(&board);
                }
                break;
            case 'j':
                if (can_move(&board, spin_left)) {
                    spin_left(&board);
                }
                break;
        }

        if (++frame % (FPS + 1) == 0) frame = 1;

        timespec_get(&timespec, TIME_UTC);
        long milli_finish = timespec.tv_nsec / 1000 / 1000;
        if (milli_finish < milli_start) milli_finish += 1000;

        // stabilize fps
        long milli_delta = DELAY_MILLI_PER_FRAME - (milli_finish - milli_start);
        if (inputKey != ERR && milli_delta > 0) {
            usleep(milli_delta * 1000);
        }
    }
}

void draw_board_frame(int field_orig_y, int field_orig_x) {
    // draw walls
    for (int i = 0; i < FIELD_HEIGHT; i++) {
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
    for (int j = 0; j < FIELD_HEIGHT; j++) {
        for (int i = 0; i < FIELD_WIDTH; i++) {
            int should_be_period = i % 2 == 0 && j % 2 == 0;

            move(orig_y + j, orig_x + i);
            attrset(COLOR_PAIR(COLOR_ID_FIELD));
            addch(should_be_period ? '.' : ' ');

            move(orig_y + j, orig_x + i + 10);
            attrset(COLOR_PAIR(COLOR_ID_FIELD));
            addch(should_be_period ? '.' : ' ');
        }
    }
}