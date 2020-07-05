#include "marathon.h"

#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#include "../graphics.h"
#include "../utils.h"

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
    MinoBlock field[20][10];
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
                        {0, 0, 0},
                        {1, 1, 1},
                        {0, 1, 0},
                },
                {
                        {0, 1, 0},
                        {0, 1, 1},
                        {0, 1, 0},
                },
                {
                        {0, 1, 0},
                        {1, 1, 1},
                        {0, 0, 0},
                },
                {
                        {0, 1, 0},
                        {1, 1, 0},
                        {0, 1, 0},
                },
        },
        .size = 3,
};

void draw_field(int y, int x);

void erase_background_of_field(int y, int x);

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
            if (assumed.dropping_mino->shape[assumed.dropping_mino_spin][j][i] == 0) continue;

            int x = assumed.dropping_mino_x + (i - assumed.dropping_mino->center_x);
            int y = assumed.dropping_mino_y + (j - assumed.dropping_mino->center_y);

            if (x < 0 || y <= 0 || 10 <= x || 20 <= y || assumed.field[y][x] != 0) return 0;
        }
    }

    return 1;
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
    int frame = 1;
    int board_x = WINDOW_WIDTH / 2 - 12, board_y = 10;

    struct timespec timespec;

    efface_window();
    draw_window_frame();
    draw_field(board_y, board_x);

    Board board = {
            .field = {},
            .dropping_mino = &MINO_T,
            .dropping_mino_x = 5,
            .dropping_mino_y = 0,
            .dropping_mino_spin = 0,
            .dropping_mass_per_second = 1,
    };

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

            erase_background_of_field(board_y, board_x);

            // render a board
            for (int y = 0; y < 20; y++) {
                for (int x = 0; x < 10; x++) {
                    if (board.field[y][x]) {
                        move(board_y + y, board_x + 2 * x);
                        attrset(COLOR_PAIR(COLOR_ID_MINO_T));
                        addstr("  ");
                    }
                }
            }

            // render a dropping mino
            for (int y = 0; y < board.dropping_mino->size; y++) {
                for (int x = 0; x < board.dropping_mino->size; x++) {
                    if (board.dropping_mino->shape[board.dropping_mino_spin][y][x]) {
                        move(
                                board_y + board.dropping_mino_y + board.dropping_mino->center_y - y,
                                board_x + 2 * (board.dropping_mino_x - board.dropping_mino->center_x + x) + 2
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

void draw_field(int y, int x) {
    // draw walls
    for (int i = 0; i < 20; i++) {
        move(y + i, x);
        attrset(COLOR_PAIR(COLOR_ID_FIELD));
        addstr("[]");

        move(y + i, x + 20 + 2);
        attrset(COLOR_PAIR(COLOR_ID_FIELD));
        addstr("[]");
    }

    // draw bottom
    for (int i = 0; i < 20 + 4; i++) {
        move(y + 20, x + i);
        attrset(COLOR_PAIR(COLOR_ID_FIELD));
        addch('T');
    }

    erase_background_of_field(y, x);
}

void erase_background_of_field(int y, int x) {
    for (int j = -2; j < 20; j++) {
        for (int i = 0; i < 10; i++) {
            int should_be_period = i % 2 == 0 && j % 2 == 0 && j >= 0;

            move(y + j, x + i + 2);
            attrset(COLOR_PAIR(COLOR_ID_FIELD));
            addch(should_be_period ? '.' : ' ');

            move(y + j, x + i + 2 + 10);
            attrset(COLOR_PAIR(COLOR_ID_FIELD));
            addch(should_be_period ? '.' : ' ');
        }
    }
}