#include "ranking.h"

#include <ncurses.h>
#include <string.h>

#include "components.h"
#include "../graphics.h"

const char *TEXT_RANKING_AA_1 = "  _.+._";
const char *TEXT_RANKING_AA_2 = "(^\\/^\\/^)";
const char *TEXT_RANKING_AA_3 = " \\@*@*@/   RANKING";
const char *TEXT_RANKING_AA_4 = " {_____}";

void disp_ranking() {
    erase();
    draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    attrset(COLOR_PAIR(COLOR_ID_FIELD));
    mvprintw(3, WINDOW_WIDTH / 2 - (int) strlen(TEXT_RANKING_AA_1) / 2 - 8, TEXT_RANKING_AA_1);
    mvprintw(4, WINDOW_WIDTH / 2 - (int) strlen(TEXT_RANKING_AA_1) / 2 - 8, TEXT_RANKING_AA_2);
    mvprintw(5, WINDOW_WIDTH / 2 - (int) strlen(TEXT_RANKING_AA_1) / 2 - 8, TEXT_RANKING_AA_3);
    mvprintw(6, WINDOW_WIDTH / 2 - (int) strlen(TEXT_RANKING_AA_1) / 2 - 8, TEXT_RANKING_AA_4);

    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    mvprintw(WINDOW_HEIGHT / 4, WINDOW_WIDTH / 4 * 1 - 3, "40LINE");
    mvprintw(WINDOW_HEIGHT / 4, WINDOW_WIDTH / 4 * 2 - 4, "MARATHON");
    mvprintw(WINDOW_HEIGHT / 4, WINDOW_WIDTH / 4 * 3 - 2, "ULTRA");

    TimeRecord records_40line[MAX_RECORDS];
    get_40line_records(records_40line);
    for (int i = 0; i < MAX_RECORDS; i++) {
        if (records_40line[i].seconds == TIME_RECORD_UNDEFINED) continue;

        mvprintw(WINDOW_HEIGHT / 4 + 2 + 2 * i, WINDOW_WIDTH / 4 - 2 - (strlen(records_40line[i].name) + 1),
                 "%s - %02d:%02d",
                 records_40line[i].name,
                 records_40line[i].seconds / 60,
                 records_40line[i].seconds % 60
        );
    }

    ScoreRecord records_marathon[MAX_RECORDS];
    get_marathon_records(records_marathon);
    for (int i = 0; i < MAX_RECORDS; i++) {
        if (records_marathon[i].score == SCORE_RECORD_UNDEFINED) continue;

        mvprintw(WINDOW_HEIGHT / 4 + 2 + 2 * i, WINDOW_WIDTH / 2 - 2 - (strlen(records_marathon[i].name) + 1),
                 "%s - %7d",
                 records_marathon[i].name,
                 records_marathon[i].score
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