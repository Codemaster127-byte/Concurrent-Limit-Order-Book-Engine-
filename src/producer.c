#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "main.h"

extern OrderQueue queue;
extern int running;
extern int order_id;

Order generate_random_order(void) {
    Order o;

    o.id = __sync_fetch_and_add(&order_id, 1);
    o.price = 100 + (rand() % 901);
    o.quantity = 1 + (rand() % 100);
    o.side = rand() % 2;

    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        o.timestamp = ts.tv_sec * 1000000000L + ts.tv_nsec;
    } else {
        o.timestamp = 0;  // used a fallback here
    }

    return o;
}

void* producer(void* arg) {

    while(running) {
        Order o = generate_random_order();

        enqueue(&queue, o);

        usleep(rand() % 1000);
    }

    return NULL;
}