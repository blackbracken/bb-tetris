#ifndef BBTETRIS_TITLE_H
#define BBTETRIS_TITLE_H

typedef enum {
    DEST_40LINE,
    DEST_MARATHON,
    DEST_ULTRA,
    DEST_RANKING,
    DEST_HELP,
    DEST_EXIT,
} MenuDestination;

MenuDestination disp_menu();

#endif
