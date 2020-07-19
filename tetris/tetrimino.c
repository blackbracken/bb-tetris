#include "tetrimino.h"

// TODO: write spinning offsets
const Tetrimino MINO_T = {
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
        .color = PURPLE,
};

const Tetrimino MINO_S = {
        .center_x =  1,
        .center_y =  1,
        .shape = {
                {
                        {0, 1, 1},
                        {1, 1, 0},
                        {0, 0, 0},
                },
                {
                        {0, 1, 0},
                        {0, 1, 1},
                        {0, 0, 1},
                },
                {
                        {0, 0, 0},
                        {0, 1, 1},
                        {1, 1, 0},
                },
                {
                        {1, 0, 0},
                        {1, 1, 0},
                        {0, 1, 0},
                },
        },
        .size = 3,
        .color = GREEN,
};

const Tetrimino MINO_Z = {
        .center_x =  1,
        .center_y =  1,
        .shape = {
                {
                        {1, 1, 0},
                        {0, 1, 1},
                        {0, 0, 0},
                },
                {
                        {0, 0, 1},
                        {0, 1, 1},
                        {0, 1, 0},
                },
                {
                        {0, 0, 0},
                        {1, 1, 0},
                        {0, 1, 1},
                },
                {
                        {0, 1, 0},
                        {1, 1, 0},
                        {1, 0, 0},
                },
        },
        .size = 3,
        .color = RED,
};

const Tetrimino MINO_L = {
        .center_x =  1,
        .center_y =  1,
        .shape = {
                {
                        {0, 0, 1},
                        {1, 1, 1},
                        {0, 0, 0},
                },
                {
                        {0, 1, 0},
                        {0, 1, 0},
                        {0, 1, 1},
                },
                {
                        {0, 0, 0},
                        {1, 1, 1},
                        {1, 0, 0},
                },
                {
                        {1, 1, 0},
                        {0, 1, 0},
                        {0, 1, 0},
                },
        },
        .size = 3,
        .color = WHITE,
};

const Tetrimino MINO_J = {
        .center_x =  1,
        .center_y =  1,
        .shape = {
                {
                        {1, 0, 0},
                        {1, 1, 1},
                        {0, 0, 0},
                },
                {
                        {0, 1, 1},
                        {0, 1, 0},
                        {0, 1, 0},
                },
                {
                        {0, 0, 0},
                        {1, 1, 1},
                        {0, 0, 1},
                },
                {
                        {0, 1, 0},
                        {0, 1, 0},
                        {1, 1, 0},
                },
        },
        .size = 3,
        .color = BLUE,
};

const Tetrimino MINO_O = {
        .center_x = 0,
        .center_y = 0,
        .shape = {
                {
                        {1, 1},
                        {1, 1}
                },
                {
                        {1, 1},
                        {1, 1}
                },
                {
                        {1, 1},
                        {1, 1}
                },
                {
                        {1, 1},
                        {1, 1}
                },
        },
        .size = 2,
        .color = YELLOW,
};

const Tetrimino MINO_I = {
        .center_x = 1,
        .center_y = 1,
        .shape = {
                {
                        {0, 0, 0, 0},
                        {1, 1, 1, 1},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {0, 0, 1, 0},
                        {0, 0, 1, 0},
                        {0, 0, 1, 0},
                        {0, 0, 1, 0},
                },
                {
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {1, 1, 1, 1},
                        {0, 0, 0, 0},
                },
                {
                        {0, 1, 0, 0},
                        {0, 1, 0, 0},
                        {0, 1, 0, 0},
                        {0, 1, 0, 0},
                }
        },
        .size = 4,
        .color = CYAN,
};