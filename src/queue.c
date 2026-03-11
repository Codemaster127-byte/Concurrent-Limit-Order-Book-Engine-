#include "main.h"


void enqueue(OrderQueue* oq,Order o){
    pthread_mutex_lock(&oq->lock);

    while(((oq->tail+1)%QUEUE_SIZE)==oq->head){

        pthread_cond_wait(&oq->not_full, &oq->lock);

    }

    oq->buffer[oq->tail]=o;
    oq->tail=(oq->tail+1)%QUEUE_SIZE;

    pthread_cond_signal(&oq->not_empty);

    pthread_mutex_unlock(&oq->lock);

}


Order dequeue(OrderQueue* oq){
    pthread_mutex_lock(&oq->lock);

    while((oq->tail)==(oq->head)){

        pthread_cond_wait(&oq->not_empty, &oq->lock);

    }

    Order order = oq->buffer[oq->head];
    oq->head = (oq->head + 1) % QUEUE_SIZE;


    pthread_cond_signal(&oq->not_full);
    
    pthread_mutex_unlock(&oq->lock);

    return order;
}

void init_queue(OrderQueue *oq) {
    oq->head = 0;
    oq->tail = 0;
    pthread_mutex_init(&oq->lock, NULL);
    pthread_cond_init(&oq->not_empty, NULL);
    pthread_cond_init(&oq->not_full, NULL);
}
