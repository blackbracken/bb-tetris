#ifndef BBTETRIS_RECORD_H
#define BBTETRIS_RECORD_H

#include <stdbool.h>

#define MAX_RECORDS 5
#define LENGTH_OF_NAME 4
#define TIME_RECORD_UNDEFINED 10e8
#define SCORE_RECORD_UNDEFINED -1

typedef enum {
    RES_NEW_RECORD,
    RES_SUCCESS,
    RES_FAILURE
} GameResult;

typedef struct {
    char name[LENGTH_OF_NAME + 1];
    int seconds;
} TimeRecord;

typedef struct {
    char name[LENGTH_OF_NAME + 1];
    int score;
} ScoreRecord;

void init_ranking();

void get_40line_records(TimeRecord records[MAX_RECORDS]);

bool is_new_40line_record(int seconds);

void insert_40line_record(const TimeRecord *rec);

void get_marathon_records(ScoreRecord records[MAX_RECORDS]);

bool is_new_marathon_record(int score);

void insert_marathon_record(const ScoreRecord *rec);

#endif
