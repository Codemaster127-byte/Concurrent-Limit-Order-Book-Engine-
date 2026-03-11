#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


#include "main.h"

extern OrderQueue queue;
extern int running;
extern int order_id;

Order generate_random_order(void) {
    Order o;

    o.id = __sync_fetch_and_add(&order_id, 1);
    o.price = rand_normal(ltp);
    o.quantity = 1 + (rand() % 100);
    o.side = (o.price<ltp)? 0:1;

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
        double wait = rand_exponential(lambda);
        usleep(wait*1e6);
    }

    return NULL;
}


double rand_normal(double ltp)
{
    double u1 = (rand() + 1.0) / (RAND_MAX + 2.0);
    double u2 = (rand() + 1.0) / (RAND_MAX + 2.0);

    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

    return ltp + z0 * (ltp*0.10);
}





double rand_exponential(double lambda)
{
    double u = (rand() + 1.0) / (RAND_MAX + 2.0);
    return -log(u) / lambda;
}