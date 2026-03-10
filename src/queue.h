#ifndef queue_h
#define queue_h
#include <time.h>

typedef struct {
    int price;
    int quantity;
    int buy_sell;// buy=0,sell=1
    time_t ct;    
}Order;

int enqueue(Order *ob);
int dnqueue(Order *ob);
int empty_queue();
int full_queue();


#endif
