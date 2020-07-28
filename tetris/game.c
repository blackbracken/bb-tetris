#include "game.h"

#include <stdlib.h>
#include <ncurses.h>

#define SIZE_OF_MINO_BAG 14

const int FPS = 30;
const int DELAY_MILLI_PER_FRAME = 1000 / FPS;

const int BLOCK_AMOUNT_IN_MINO = 4;

typedef struct {
    int x, y;
} MinoLocation;

// -- board controllers --
bool put_and_try_next(Board *board);

bool remove_line_if_completed(Board *board, int removed_y);

bool try_spawn_mino(Board *board, const Tetrimino *next);
// --

// -- unsafe movements on board --
void drop_mino_once(Board *board);

void move_left(Board *board);

void move_right(Board *board);

void spin_left(Board *board, int offset_idx);

void spin_right(Board *board, int offset_idx);
// --

// -- verifications --
bool can_move(const Board *board, void (*movement)(Board *));

bool will_overlap(Board *board);

int search_offset_idx_to_spin(const Board *board, bool is_clockwise);

void calc_dropping_mino_locations_on_field(Board *board, MinoLocation *locations);
// --

// -- methods for mino bag --
void gen_mino_bag(MinoBag *bag);

void insert_shuffled_minos(MinoBag *bag, int lead);

const Tetrimino *pop_mino(MinoBag *bag);
// --

void make_board(Board *board) {
    for (int j = 0; j < FIELD_HEIGHT; j++) {
        for (int i = 0; i < FIELD_WIDTH; i++) {
            board->field[j][i] = AIR;
        }
    }
    board->held_mino = NULL;
    board->did_already_hold = false;
    board->dropping_mass_per_second = 1;
    board->lockdown_count = 0;
    gen_mino_bag(&board->bag);

    Statistics statistics = {
            .score = 0,
            .total_removed_lines = 0,
            .elapsed_seconds = 0,
    };
    board->statistics = statistics;

    board->ren_count = 0;
    board->removing_reward = NONE;
    board->on_ready_back_to_back = false;
    board->did_back_to_back = false;

    board->on_immediately_after_rotating = false;

    try_spawn_mino(board, pop_mino(&board->bag));
}

bool render(Board *board, int frame) {
    if (frame == FPS) {
        board->statistics.elapsed_seconds++;
    }

    if (!can_move(board, drop_mino_once)) {
        board->lockdown_count++;
    }

    if (board->lockdown_count >= FPS / 2) {
        board->lockdown_count = 0;

        if (!put_and_try_next(board)) {
            return false;
        }
    }

    if (frame % (FPS / board->dropping_mass_per_second) == 0) {
        if (can_move(board, drop_mino_once)) {
            drop_mino_once(board);
        }
    }

    return true;
}

const Tetrimino *peek_next(Board *board, int order) {
    if (order < 0 || 7 <= order) return NULL;

    MinoBag *bag = &board->bag;
    int generated_size = bag->done_tail > bag->head
                         ? bag->done_tail - bag->head
                         : bag->done_tail + SIZE_OF_MINO_BAG - bag->head;

    if (generated_size < order) {
        insert_shuffled_minos(bag, 7 < bag->head ? 0 : 7);
    }

    return bag->blocks[(bag->head + order) % SIZE_OF_MINO_BAG];
}

void try_move_left(Board *board) {
    if (can_move(board, move_left)) {
        move_left(board);
        board->lockdown_count = 0;
    }
}

void try_move_right(Board *board) {
    if (can_move(board, move_right)) {
        move_right(board);
        board->lockdown_count = 0;
    }
}

void try_spin_left(Board *board) {
    int offset_idx = search_offset_idx_to_spin(board, false);
    if (offset_idx != -1) {
        spin_left(board, offset_idx);
        board->lockdown_count = 0;
    }
}

void try_spin_right(Board *board) {
    int offset_idx = search_offset_idx_to_spin(board, true);
    if (offset_idx != -1) {
        spin_right(board, offset_idx);
        board->lockdown_count = 0;
    }
}

void try_hold(Board *board) {
    if (board->did_already_hold) return;

    const Tetrimino *dropping = board->dropping_mino;

    try_spawn_mino(board, board->held_mino != NULL ? board->held_mino : pop_mino(&board->bag));
    board->held_mino = dropping;
    board->did_already_hold = true;
}

void drop_softly(Board *board) {
    if (can_move(board, drop_mino_once)) {
        drop_mino_once(board);
    }
}

void drop_hardly(Board *board) {
    while (can_move(board, drop_mino_once)) drop_softly(board);

    board->lockdown_count = 1000;
}

bool put_and_try_next(Board *board) {
    if (can_move(board, drop_mino_once)) return true;

    int removed_lines_count = 0;
    MinoLocation locations[BLOCK_AMOUNT_IN_MINO];

    int filled_corner = 0;
    for (int j = -1; j <= 1; j += 2) {
        for (int i = -1; i <= 1; i += 2) {
            int x_on_field = board->dropping_mino_x + i;
            int y_on_field = board->dropping_mino_y + j;

            if (x_on_field < 0 || y_on_field < 0
                || FIELD_WIDTH <= x_on_field || FIELD_HEIGHT <= y_on_field
                || board->field[y_on_field][x_on_field] != AIR) {
                filled_corner++;
            }
        }
    }

    // remove lines
    calc_dropping_mino_locations_on_field(board, locations);
    for (int i = 0; i < BLOCK_AMOUNT_IN_MINO; i++) {
        int x_on_field = locations[i].x;
        int y_on_field = locations[i].y;

        if (y_on_field >= FIELD_HEIGHT) {
            return false;
        } else {
            board->field[y_on_field][x_on_field] = board->dropping_mino->color;

            if (remove_line_if_completed(board, y_on_field)) removed_lines_count++;
        }
    }
    board->statistics.total_removed_lines += removed_lines_count;

    // count ren
    if (removed_lines_count == 0) {
        board->ren_count = 0;
    } else {
        board->ren_count++;
    }

    // attach removing rewards
    board->removing_reward = NONE;
    bool did_back_to_back = removed_lines_count == 4
                            || (board->on_immediately_after_rotating
                                && removed_lines_count > 0
                                && board->dropping_mino == &MINO_T
                                && 3 <= filled_corner
                            );
    if (did_back_to_back) {
        switch (removed_lines_count) {
            case 4:
                board->removing_reward = TETRIS;
                break;
            case 3:
                board->removing_reward = TSPIN_TRIPLE;
                break;
            case 2:
                board->removing_reward = TSPIN_DOUBLE;
                break;
            case 1:
                board->removing_reward = TSPIN_SINGLE;
                break;
            default:
                break;
        }
    }
    bool did_perfect_clear = true;
    for (int j = 0; j < FIELD_HEIGHT; j++) {
        for (int i = 0; i < FIELD_WIDTH; i++) {
            if (board->field[j][i] != AIR) {
                did_perfect_clear = false;
            }
        }
    }
    if (did_perfect_clear) {
        board->removing_reward = PERFECT_CLEAR;
    }

    // attach back-to-back
    board->did_back_to_back = false;
    if (removed_lines_count > 0) {
        if (!did_back_to_back) {
            board->on_ready_back_to_back = false;
        } else {
            if (board->on_ready_back_to_back) {
                board->did_back_to_back = true;
            }

            board->on_ready_back_to_back = true;
        }
    }

    if (!try_spawn_mino(board, pop_mino(&board->bag))) {
        return false;
    }

    board->did_already_hold = false;

    return true;
}

bool remove_line_if_completed(Board *board, int removed_y) {
    for (int i = 0; i < FIELD_WIDTH; i++) {
        if (board->field[removed_y][i] == AIR) return false;
    }

    // shift lines above removed line to lower.
    for (int j = removed_y - 1; 0 < j; j--) {
        for (int i = 0; i < FIELD_WIDTH; i++) {
            board->field[j + 1][i] = board->field[j][i];
        }
    }
    for (int i = 0; i < FIELD_WIDTH; i++) {
        board->field[0][i] = AIR;
    }

    return true;
}

bool try_spawn_mino(Board *board, const Tetrimino *mino) {
    board->dropping_mino = mino;
    board->dropping_mino_x = 4;
    board->dropping_mino_y = 2;
    board->dropping_mino_spin = 0;

    // shift to upper if overlapped
    if (will_overlap(board)) {
        board->dropping_mino_y = 1;
    }
    // it can't continue if overlapped yet
    if (will_overlap(board)) {
        return false;
    }

    return true;
}

void drop_mino_once(Board *board) {
    board->dropping_mino_y++;
    board->on_immediately_after_rotating = false;
}

void move_left(Board *board) {
    board->dropping_mino_x--;
    board->on_immediately_after_rotating = false;
}

void move_right(Board *board) {
    board->dropping_mino_x++;
    board->on_immediately_after_rotating = false;
}

void spin_left(Board *board, int offset_idx) {
    board->dropping_mino_spin = (board->dropping_mino_spin + 3) % 4;

    const SpinOffset *offset = &board->dropping_mino->spin_offsets[board->dropping_mino_spin][offset_idx];
    board->dropping_mino_x -= offset->x;
    board->dropping_mino_y -= offset->y;

    board->on_immediately_after_rotating = true;
}

void spin_right(Board *board, int offset_idx) {
    const SpinOffset *offset = &board->dropping_mino->spin_offsets[board->dropping_mino_spin][offset_idx];
    board->dropping_mino_x += offset->x;
    board->dropping_mino_y += offset->y;

    board->dropping_mino_spin = (board->dropping_mino_spin + 1) % 4;

    board->on_immediately_after_rotating = true;
}

bool can_move(const Board *board, void (*movement)(Board *)) {
    Board assumed = *board;
    movement(&assumed);

    MinoLocation locations[BLOCK_AMOUNT_IN_MINO];
    calc_dropping_mino_locations_on_field(&assumed, locations);
    for (int i = 0; i < BLOCK_AMOUNT_IN_MINO; i++) {
        int x_on_field = locations[i].x;
        int y_on_field = locations[i].y;

        if (x_on_field < 0 || y_on_field < 0
            || FIELD_WIDTH <= x_on_field || FIELD_HEIGHT <= y_on_field
            || assumed.field[y_on_field][x_on_field] != AIR) {
            return false;
        }
    }

    return true;
}

bool will_overlap(Board *board) {
    MinoLocation locations[BLOCK_AMOUNT_IN_MINO];
    calc_dropping_mino_locations_on_field(board, locations);
    for (int i = 0; i < BLOCK_AMOUNT_IN_MINO; i++) {
        int x_on_field = locations[i].x;
        int y_on_field = locations[i].y;

        if (board->field[y_on_field][x_on_field] != AIR) {
            return true;
        }
    }

    return false;
}

int search_offset_idx_to_spin(const Board *board, bool is_clockwise) {
    for (int offset_idx = 0; offset_idx < 5; offset_idx++) {
        Board assumed = *board;

        if (is_clockwise) {
            spin_right(&assumed, offset_idx);
        } else {
            spin_left(&assumed, offset_idx);
        }

        MinoLocation locations[BLOCK_AMOUNT_IN_MINO];
        calc_dropping_mino_locations_on_field(&assumed, locations);

        bool cracked = false;
        for (int i = 0; i < BLOCK_AMOUNT_IN_MINO; i++) {
            int x_on_field = locations[i].x;
            int y_on_field = locations[i].y;

            if (x_on_field < 0 || y_on_field < 0
                || FIELD_WIDTH <= x_on_field || FIELD_HEIGHT <= y_on_field
                || assumed.field[y_on_field][x_on_field] != AIR) {
                cracked = true;
                break;
            }
        }
        if (!cracked) return offset_idx;
    }
    return -1;
}

void calc_dropping_mino_locations_on_field(Board *board, MinoLocation *locations) {
    int loc_idx = 0;

    for (int j = 0; j < board->dropping_mino->size; j++) {
        for (int i = 0; i < board->dropping_mino->size; i++) {
            if (board->dropping_mino->shape[board->dropping_mino_spin][j][i] == 0) {
                continue;
            }

            MinoLocation loc_on_field = {
                    .x = board->dropping_mino_x + (i - board->dropping_mino->center_x),
                    .y = board->dropping_mino_y + (j - board->dropping_mino->center_y),
            };
            locations[loc_idx++] = loc_on_field;
        }
    }
}

void gen_mino_bag(MinoBag *bag) {
    bag->head = 0;
    bag->done_tail = 0;
    insert_shuffled_minos(bag, 0);
}

void insert_shuffled_minos(MinoBag *bag, int lead) {
    if (lead % 7 != 0) return;

    bag->blocks[lead + 0] = &MINO_T;
    bag->blocks[lead + 1] = &MINO_O;
    bag->blocks[lead + 2] = &MINO_S;
    bag->blocks[lead + 3] = &MINO_Z;
    bag->blocks[lead + 4] = &MINO_L;
    bag->blocks[lead + 5] = &MINO_J;
    bag->blocks[lead + 6] = &MINO_I;

    for (int i = 0; i < 7; i++) {
        int left = lead + i;
        int right = lead + (rand() % 7);

        const Tetrimino *temp = bag->blocks[left];
        bag->blocks[left] = bag->blocks[right];
        bag->blocks[right] = temp;
    }

    bag->done_tail = lead + 6;
}

const Tetrimino *pop_mino(MinoBag *bag) {
    int next_idx = (bag->head + 1) % SIZE_OF_MINO_BAG;

    if (next_idx == bag->done_tail) {
        insert_shuffled_minos(bag, next_idx + 1);
    }

    const Tetrimino *mino = bag->blocks[bag->head];
    bag->head = next_idx;
    return mino;
}