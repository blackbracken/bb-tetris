#include "ranking.h"

#include <ncurses.h>
#include <string.h>

#include "components.h"
#include "../graphics.h"

void disp_ranking() {
    erase();
    draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    mvprintw(WINDOW_HEIGHT / 4, WINDOW_WIDTH / 4 * 1 - 3, "40LINE");
    mvprintw(WINDOW_HEIGHT / 4, WINDOW_WIDTH / 4 * 2 - 4, "MARATHON");
    mvprintw(WINDOW_HEIGHT / 4, WINDOW_WIDTH / 4 * 3 - 2, "ULTRA");

    TimeRecord records[MAX_RECORDS];
    get_40line_records(records);
    for (int i = 0; i < MAX_RECORDS; i++) {
        if (records[i].seconds == TIME_RECORD_UNDEFINED) continue;

        mvprintw(WINDOW_HEIGHT / 4 + 2 + 2 * i, WINDOW_WIDTH / 4 - 2 - (strlen(records[i].name) + 1),
                 "%s - %02d:%02d",
                 records[i].name,
                 records[i].seconds / 60,
                 records[i].seconds % 60
        );
    }

    draw_mino(&MINO_T, WINDOW_HEIGHT - 12, WINDOW_WIDTH / 2 + 0, 3);
    draw_mino(&MINO_J, WINDOW_HEIGHT - 12, WINDOW_WIDTH / 2 - 11, 0);
    draw_mino(&MINO_S, WINDOW_HEIGHT - 12 + 1, WINDOW_WIDTH / 2 + 9, 0);

    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    mvprintw(WINDOW_HEIGHT - 4, WINDOW_WIDTH / 2 - strlen(TEXT_RETURN) / 2, TEXT_RETURN);

    refresh();

    while (getch() != ' ');
}