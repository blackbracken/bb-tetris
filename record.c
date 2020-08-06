#include "record.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    REC_40LINE,
    REC_MARATHON,
    REC_ULTRA,

    NUM_OF_REC_KIND,
} Record;

const int LOG_SIZE = 1024;
const char *LOG_FILE_NAME = "ranking.dat";

void read_raw_records(char *raw_text, Record rec);

void write_raw_records(char *raw_text, Record rec);

TimeRecord to_time_record(const char *pair_text);

ScoreRecord to_score_record(const char *pair_text);

int compare_time_record(const void *left, const void *right);

int compare_score_record(const void *left, const void *right);

void init_ranking() {
    FILE *file = fopen(LOG_FILE_NAME, "r");
    bool should_initialize = file == NULL;
    if (should_initialize) {
        file = fopen(LOG_FILE_NAME, "w");
        fputs("///\n", file);
    }
    fclose(file);

    if (should_initialize) {
        TimeRecord sample_40line_rec = {.name = "SMPL", .seconds=120};
        insert_40line_record(&sample_40line_rec);
        ScoreRecord sample_marathon_rec = {.name="SMPL", .score=25000};
        insert_marathon_record(&sample_marathon_rec);
        ScoreRecord sample_ultra_rec = {.name="SMPL", .score=10000};
        insert_ultra_record(&sample_ultra_rec);
    }
}

void get_40line_records(TimeRecord records[MAX_RECORDS]) {
    char raw_records[LOG_SIZE / NUM_OF_REC_KIND];
    read_raw_records(raw_records, REC_40LINE);

    records[0] = to_time_record(strtok(raw_records, ","));
    for (int i = 1; i < MAX_RECORDS; i++) {
        records[i] = to_time_record(strtok(NULL, ","));
    }
    qsort(records, MAX_RECORDS, sizeof(TimeRecord), compare_time_record);
}

bool is_new_40line_record(int seconds) {
    TimeRecord records[MAX_RECORDS];
    get_40line_records(records);

    return records[MAX_RECORDS - 1].seconds > seconds;
}

void insert_40line_record(const TimeRecord *rec) {
    TimeRecord records[MAX_RECORDS + 1];
    get_40line_records(records);

    records[MAX_RECORDS] = *rec;
    qsort(records, MAX_RECORDS + 1, sizeof(TimeRecord), compare_time_record);

    char raw_text[LOG_SIZE / NUM_OF_REC_KIND];
    memset(raw_text, '\0', LOG_SIZE / NUM_OF_REC_KIND);
    for (int i = 0; i < MAX_RECORDS; i++) {
        char raw_rec[32];
        sprintf(raw_rec, "%d=%s,", records[i].seconds, records[i].name);
        strcat(raw_text, raw_rec);
    }

    write_raw_records(raw_text, REC_40LINE);
}

void get_marathon_records(ScoreRecord records[MAX_RECORDS]) {
    char raw_records[LOG_SIZE / NUM_OF_REC_KIND];
    read_raw_records(raw_records, REC_MARATHON);

    records[0] = to_score_record(strtok(raw_records, ","));
    for (int i = 1; i < MAX_RECORDS; i++) {
        records[i] = to_score_record(strtok(NULL, ","));
    }
    qsort(records, MAX_RECORDS, sizeof(ScoreRecord), compare_score_record);
}

bool is_new_marathon_record(int score) {
    ScoreRecord records[MAX_RECORDS];
    get_marathon_records(records);

    return records[MAX_RECORDS - 1].score < score;
}

void insert_marathon_record(const ScoreRecord *rec) {
    ScoreRecord records[MAX_RECORDS + 1];
    get_marathon_records(records);

    records[MAX_RECORDS] = *rec;
    qsort(records, MAX_RECORDS + 1, sizeof(ScoreRecord), compare_score_record);

    char raw_text[LOG_SIZE / NUM_OF_REC_KIND];
    memset(raw_text, '\0', LOG_SIZE / NUM_OF_REC_KIND);
    for (int i = 0; i < MAX_RECORDS; i++) {
        char raw_rec[32];
        sprintf(raw_rec, "%d=%s,", records[i].score, records[i].name);
        strcat(raw_text, raw_rec);
    }

    write_raw_records(raw_text, REC_MARATHON);
}

void get_ultra_records(ScoreRecord records[MAX_RECORDS]) {
    char raw_records[LOG_SIZE / NUM_OF_REC_KIND];
    read_raw_records(raw_records, REC_ULTRA);

    records[0] = to_score_record(strtok(raw_records, ","));
    for (int i = 1; i < MAX_RECORDS; i++) {
        records[i] = to_score_record(strtok(NULL, ","));
    }
    qsort(records, MAX_RECORDS, sizeof(ScoreRecord), compare_score_record);
}

bool is_new_ultra_records(int score) {
    ScoreRecord records[MAX_RECORDS];
    get_ultra_records(records);

    return records[MAX_RECORDS - 1].score < score;
}

void insert_ultra_record(const ScoreRecord *rec) {
    ScoreRecord records[MAX_RECORDS + 1];
    get_ultra_records(records);

    records[MAX_RECORDS] = *rec;
    qsort(records, MAX_RECORDS + 1, sizeof(ScoreRecord), compare_score_record);

    char raw_text[LOG_SIZE / NUM_OF_REC_KIND];
    memset(raw_text, '\0', LOG_SIZE / NUM_OF_REC_KIND);
    for (int i = 0; i < MAX_RECORDS; i++) {
        char raw_rec[32];
        sprintf(raw_rec, "%d=%s,", records[i].score, records[i].name);
        strcat(raw_text, raw_rec);
    }

    write_raw_records(raw_text, REC_ULTRA);
}

void read_raw_records(char *raw_text, Record rec) {
    FILE *file = fopen(LOG_FILE_NAME, "r");
    if (file == NULL) {
        fputs("Failed to open ranking.dat.", stderr);
        exit(1);
    }
    char log[LOG_SIZE];
    fgets(log, LOG_SIZE, file);

    char *raws[NUM_OF_REC_KIND];
    raws[0] = strtok(log, "/");
    for (int i = 1; i < NUM_OF_REC_KIND; i++) {
        raws[i] = strtok(NULL, "/");
    }

    strcpy(raw_text, raws[(int) rec]);

    fclose(file);
}

void write_raw_records(char *raw_text, Record rec) {
    FILE *file = fopen(LOG_FILE_NAME, "r");
    if (file == NULL) {
        fputs("Failed to open ranking.dat.", stderr);
        exit(1);
    }
    char log[LOG_SIZE];
    fgets(log, LOG_SIZE, file);

    char *raw_records[NUM_OF_REC_KIND];

    raw_records[0] = strtok(log, "/");
    if ((int) rec == 0) raw_records[0] = raw_text;
    for (int i = 1; i < NUM_OF_REC_KIND; i++) {
        raw_records[i] = strtok(NULL, "/");
        if ((int) rec == i) raw_records[i] = raw_text;
    }

    fclose(file);

    file = fopen(LOG_FILE_NAME, "w");
    for (int i = 0; i < NUM_OF_REC_KIND; i++) {
        fprintf(file, "%s/", raw_records[i]);
    }
    fputs("\n", file);

    fclose(file);
}

TimeRecord to_time_record(const char *pair_text) {
    if (pair_text != NULL) {
        TimeRecord rec;
        if (sscanf(pair_text, "%d=%s", &rec.seconds, rec.name) == 2) {
            return rec;
        }
    }

    TimeRecord rec = {
            .name = "NONE",
            .seconds = TIME_RECORD_UNDEFINED,
    };
    return rec;
}

ScoreRecord to_score_record(const char *pair_text) {
    if (pair_text != NULL) {
        ScoreRecord rec;
        if (sscanf(pair_text, "%d=%s", &rec.score, rec.name) == 2) {
            return rec;
        }
    }

    ScoreRecord rec = {
            .name = "NONE",
            .score = SCORE_RECORD_UNDEFINED,
    };
    return rec;
}

int compare_time_record(const void *left, const void *right) {
    return ((TimeRecord *) left)->seconds - ((TimeRecord *) right)->seconds;
}

int compare_score_record(const void *left, const void *right) {
    return ((ScoreRecord *) right)->score - ((ScoreRecord *) left)->score;
}