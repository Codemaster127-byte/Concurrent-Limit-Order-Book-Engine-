#ifndef METRICS_H
#define METRICS_H

#include <time.h>

#define MAX_ORDERS 1000000

typedef struct {
    long latencies[MAX_ORDERS];
    int count;

    struct timespec start_time;
    struct timespec end_time;
} Metrics;

void metrics_init(Metrics *m);
void metrics_record_latency(Metrics *m, long latency);
void metrics_start(Metrics *m);
void metrics_end(Metrics *m);
void metrics_report(Metrics *m);

#endif