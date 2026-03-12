#include "main.h"
#include "metrics.h"

extern OrderQueue queue;
extern int running;
extern Metrics metrics;

void *matching_engine(void *arg) {
    while(running) {
        Order o = dequeue(&queue);
        process_order(&o);

        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);

        long latency = (now.tv_sec - o.created_time.tv_sec) * 1000000000L + (now.tv_nsec - o.created_time.tv_nsec); // nanosecond timestamp field reintroduced for latency

        metrics_record_latency(&metrics, latency);
    }

    return NULL;
}