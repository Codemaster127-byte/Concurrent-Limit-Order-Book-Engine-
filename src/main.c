#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "main.h"
#include "metrics.h"

OrderQueue queue;
OrderBook orderbook;
Metrics metrics;

int running = 1;
int order_id = 1;

int main() {
    srand(time(NULL));

    // initialise metrics before starting threads
    metrics_init(&metrics);
    metrics_start(&metrics);

    init_queue(&queue);

    orderbook.buy_count = 0;
    orderbook.sell_count = 0;

    pthread_t producers[3];
    pthread_t engine;

    /* create producer threads (used 4 here, can try any reasonable number) */
    for (int i = 0; i < 3; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    /* create matching engine thread */
    pthread_create(&engine, NULL, matching_engine, NULL);

    /* wait for threads (runs indefinitely)*/
    for (int i = 0; i < 3; i++) {
        pthread_join(producers[i], NULL);
    }

    pthread_join(engine, NULL);

    metrics_end(&metrics); // after all threads have been closed
    metrics_report(&metrics);

    return 0;
}