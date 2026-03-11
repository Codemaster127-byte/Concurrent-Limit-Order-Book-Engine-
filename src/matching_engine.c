#include "main.h"

extern OrderQueue queue;
extern int running;

void *matching_engine(void *arg) {
    while(running) {
        Order o = dequeue(&queue);

        process_order(&o);
    }

    return NULL;
}