#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "main.h"

OrderQueue queue;
OrderBook orderbook;

int running = 1;
int order_id = 1;

double lambda = 1.0; 

_Atomic double ltp = 800.00;


int main() {
    srand(time(NULL));

    init_queue(&queue);
    atomic_store(&ltp,800.0);

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

    return 0;
}