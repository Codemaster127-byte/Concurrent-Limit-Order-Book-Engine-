#ifndef queue_h
#define queue_h
#define QUEUE_SIZE 1000
#define MAX_ORDERS 1000
#include <time.h>
#include<pthread.h>


typedef struct {
    int id;
    int price;
    int quantity;
    int side;        // 0 = BUY, 1 = SELL
    long timestamp;
} Order;


typedef struct {
    Order buffer[QUEUE_SIZE];
    int head;
    int tail;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} OrderQueue;

typedef struct {
    Order buy_orders[MAX_ORDERS];
    Order sell_orders[MAX_ORDERS];
    int buy_count;
    int sell_count;

    pthread_mutex_t buy_lock;
    pthread_mutex_t sell_lock;

} OrderBook;


#endif