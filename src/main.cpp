#include "main.h"

#include <iostream>

int main() {
    OrderBook book;

    process_order(&book, {1, 100, 10, SELL});
    process_order(&book, {2, 100, 4, SELL});
    auto trades = process_order(&book, {3, 101, 12, BUY});

    for (const Trade& trade : trades) {
        std::cout << "trade buy=" << trade.buy_id
                  << " sell=" << trade.sell_id
                  << " price=" << trade.price
                  << " quantity=" << trade.quantity << '\n';
    }

    return 0;
}
