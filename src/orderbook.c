#include <stdio.h>
#include "main.h"

extern OrderBook orderbook;

void process_order(Order *o) {
    int i;

    if (o->side == BUY) {
        for (i = 0; i < orderbook.sell_count; i++) {
            Order *sell = &orderbook.sell_orders[i];

            if (sell->price <= o->price) {
                int traded_qty = (o->quantity < sell->quantity) ? o->quantity : sell->quantity;

                printf("TRADE: BUY %d matched with SELL %d | price=%d qty=%d\n", o->id, sell->id, sell->price, traded_qty);

                o->quantity -= traded_qty;
                sell->quantity -= traded_qty;

                /* remove sell order if filled */
                if (sell->quantity == 0) {
                    orderbook.sell_orders[i] = orderbook.sell_orders[orderbook.sell_count - 1];

                    orderbook.sell_count--;
                    i--;
                }

                if (o->quantity == 0)
                    return;
            }
        }

        /* adding remaining BUY orders to the book */

        if (orderbook.buy_count < MAX_ORDERS) {
            orderbook.buy_orders[orderbook.buy_count++] = *o;
        }
    }
    else {
        for (i = 0; i < orderbook.buy_count; i++) {
            Order *buy = &orderbook.buy_orders[i];

            if (buy->price >= o->price) {
                int traded_qty = (o->quantity < buy->quantity) ? o->quantity : buy->quantity;

                printf("TRADE: SELL %d matched with BUY %d | price=%d qty=%d\n", o->id, buy->id, buy->price, traded_qty);

                o->quantity -= traded_qty;
                buy->quantity -= traded_qty;

                /* remove buy order if filled */
                if (buy->quantity == 0) {
                    orderbook.buy_orders[i] = orderbook.buy_orders[orderbook.buy_count - 1];

                    orderbook.buy_count--;
                    i--;
                }

                if (o->quantity == 0)
                    return;
            }
        }

        /* adding remaining SELL orders to the book */

        if (orderbook.sell_count < MAX_ORDERS) {
            orderbook.sell_orders[orderbook.sell_count++] = *o;
        }
    }
}