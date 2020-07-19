#ifndef BBTETRIS_TITLE_H
#define BBTETRIS_TITLE_H

typedef enum {
    DEST_MARATHON_40,
    DEST_MARATHON_150,
    DEST_RANKING,
    DEST_HELP,
    DEST_EXIT,
} MenuDestination;

MenuDestination disp_menu();

#endif
