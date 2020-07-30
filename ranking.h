#ifndef BBTETRIS_RANKING_H
#define BBTETRIS_RANKING_H

#include <stdbool.h>

#define MAX_RECORDS 5
#define LENGTH_OF_NAME 4
#define TIME_RECORD_UNDEFINED 10e8

typedef struct {
    char name[LENGTH_OF_NAME + 1];
    int seconds;
} TimeRecord;

void init_ranking();

void get_40line_records(TimeRecord records[MAX_RECORDS]);

bool is_new_40line_record(int seconds);

void insert_40line_record(const TimeRecord *rec);

TimeRecord to_time_record(const char *pair_text);

#endif
