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
    MinoBlock table[20][10];
    Tetrimino *falling_mino;
    int falling_mino_x, falling_mino_y;
    int falling_mino_spin;
    int falling_mass_per_second;
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
                        {1, 1, 0},
                        {0, 1, 0},
                },
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
        },
        .size = 3,
};

void draw_field(int y, int x);

void erase_background_of_field(int y, int x);

_Noreturn void start_marathon(int lines) {
    int frame = 1;
    int board_x = WINDOW_WIDTH / 2 - 12, board_y = 10;

    struct timespec timespec;

    efface_window();
    draw_frame();
    draw_field(board_y, board_x);

    Board board = {
            .table = {},
            .falling_mino = &MINO_T,
            .falling_mino_x = 10,
            .falling_mino_y = 0,
            .falling_mino_spin = 0,
            .falling_mass_per_second = 1,
    };

    while (1) {
        timespec_get(&timespec, TIME_UTC);
        long milli_start = timespec.tv_nsec / 1000 / 1000;

        timeout(DELAY_MILLI_PER_FRAME);

        int inputKey = getch();

        { // routine
            if (frame % (FPS / board.falling_mass_per_second) == 0) {
                // TODO: put mino
                if (board.falling_mino_y < 20) board.falling_mino_y++;
            }

            erase_background_of_field(board_y, board_x);

            // render a board
            for (int y = 0; y < 20; y++) {
                for (int x = 0; x < 10; x++) {
                    if (board.table[y][x]) {
                        move(board_y + y, board_x + 2 * x);
                        attrset(COLOR_PAIR(COLOR_ID_MINO_T));
                        addstr("  ");
                    }
                }
            }

            // render a falling mino
            for (int j = 0; j < board.falling_mino->size; j++) {
                for (int i = 0; i < board.falling_mino->size; i++) {
                    if (board.falling_mino->shape[board.falling_mino_spin][j][i]) {
                        move(
                                board_y + board.falling_mino_y + board.falling_mino->center_y - j - 1,
                                board_x + board.falling_mino_x + board.falling_mino->center_x - i
                        );
                        attrset(COLOR_PAIR(COLOR_ID_MINO_T));
                        addstr("  ");
                    }
                }
            }

            board.falling_mino->shape;
        }

        refresh();

        switch (inputKey) {
            case 'a':
            case KEY_LEFT:
                board.falling_mino_x = max(board.falling_mino_x - 1, 0);
                break;
            case 'd':
            case KEY_RIGHT:
                board.falling_mino_x = min(board.falling_mino_x + 1, 10);
                break;
            case 'w':
            case KEY_UP:
                board.falling_mino_y = 20;
                break;
            case 's':
            case KEY_DOWN:
                board.falling_mino_y += 1;
                break;
            case 'k':
                board.falling_mino_spin = (board.falling_mino_spin + 1) % 4;
                break;
            case 'j':
                board.falling_mino_spin = (board.falling_mino_spin + 3) % 4;
                break;
        }

        if (++frame % (FPS + 1) == 0) frame = 1;

        timespec_get(&timespec, TIME_UTC);
        long milli_finish = timespec.tv_nsec / 1000 / 1000;
        if (milli_finish < milli_start) milli_finish += 1000;

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
    for (int j = -6; j < 20; j++) {
        for (int i = 0; i < 10; i++) {
            int should_be_period = i % 2 == 0 && j % 2 == 0 && j >= 0;

            move(y + j, x + i + 2);
            attrset(COLOR_PAIR(COLOR_ID_FIELD));
            addch(should_be_period ? '.' : ' ');

            move(y + j, x + i + 12);
            attrset(COLOR_PAIR(COLOR_ID_FIELD));
            addch(should_be_period ? '.' : ' ');
        }
    }
}