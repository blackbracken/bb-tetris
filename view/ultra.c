#include "ultra.h"

#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#include "../graphics.h"
#include "../tetris/game.h"
#include "components.h"

const int TIMER_SECONDS = 3 * 60;

void start_ultra() {
    int field_orig_x = WINDOW_WIDTH / 2 - 24;
    int field_orig_y = 10;

    // prepare the view
    timeout(DELAY_MILLI_PER_FRAME);
    erase();
    draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    Board board;
    make_board(&board);

    int frame = 1;
    struct timespec delta_timespec;
    while (true) {
        timespec_get(&delta_timespec, TIME_UTC);
        long milli_start = delta_timespec.tv_nsec / 1000 / 1000;

        bool is_buried = !render(&board, frame);

        int level = board.statistics.total_removed_lines / 10 + 1;
        board.dropping_mass_per_second = level;

        draw_field(&board, field_orig_y, field_orig_x);
        draw_hold(&board, field_orig_y, field_orig_x - 2 - 6);
        draw_next(&board, field_orig_y, field_orig_x + 2 * FIELD_WIDTH + 2 + 4);
        draw_rewards(&board, field_orig_y + FIELD_HEIGHT + 1, field_orig_x);
        refresh();

        if (is_buried || board.statistics.elapsed_seconds >= TIMER_SECONDS) {
            // TODO: show result
            return;
        }

        int input_key = getch();
        send_input(&board, input_key);

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