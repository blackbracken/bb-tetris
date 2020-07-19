#ifndef BBTETRIS_TETRIMINO_H
#define BBTETRIS_TETRIMINO_H

typedef enum {
    RED = 21,
    WHITE = 22,
    YELLOW = 23,
    GREEN = 24,
    CYAN = 25,
    BLUE = 26,
    PURPLE = 27,
    AIR = 28,
} MinoBlock;

typedef struct {
    const int center_x, center_y;
    const int shape[4][4][4];
    const int size;
    const int spin_offsets[4][5][2];
    const MinoBlock color;
} Tetrimino;

extern const Tetrimino MINO_T;
extern const Tetrimino MINO_O;
extern const Tetrimino MINO_S;
extern const Tetrimino MINO_Z;
extern const Tetrimino MINO_L;
extern const Tetrimino MINO_J;
extern const Tetrimino MINO_I;

#endif
