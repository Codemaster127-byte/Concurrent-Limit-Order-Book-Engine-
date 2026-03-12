#include "metrics.h"
#include <stdio.h>
#include <stdlib.h>

int cmp_long(const void *a, const void *b) {
    long x = *(long*)a;
    long y = *(long*)b;
    return (x > y) - (x < y);
}

void metrics_init(Metrics *m) {
    m->count = 0;
}

void metrics_start(Metrics *m)
{
    clock_gettime(CLOCK_MONOTONIC, &m->start_time);
}

void metrics_end(Metrics *m)
{
    clock_gettime(CLOCK_MONOTONIC, &m->end_time);
}

void metrics_record_latency(Metrics *m, long latency)
{
    if (m->count < MAX_ORDERS)
        m->latencies[m->count++] = latency;
}

void metrics_report(Metrics *m)
{
    qsort(m->latencies, m->count, sizeof(long), cmp_long);

    long p50 = m->latencies[(int)(m->count * 0.50)];
    long p95 = m->latencies[(int)(m->count * 0.95)];
    long p99 = m->latencies[(int)(m->count * 0.99)];

    double total_time =
        (m->end_time.tv_sec - m->start_time.tv_sec) +
        (m->end_time.tv_nsec - m->start_time.tv_nsec) / 1e9;

    double throughput = m->count / total_time;

    printf("\n===== PERFORMANCE METRICS =====\n");
    printf("Orders processed: %d\n", m->count);
    printf("Total runtime: %.4f sec\n", total_time);
    printf("Throughput: %.2f orders/sec\n", throughput);

    printf("\nLatency (nanoseconds)\n");
    printf("p50: %ld\n", p50);
    printf("p95: %ld\n", p95);
    printf("p99: %ld\n", p99);

}

