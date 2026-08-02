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

    replace_order(&book, 2, 102, 5);
    std::cout << "replaced order 2 quantity=" << book.sells.begin()->second.total << '\n';
    cancel_order(&book, 2);
    std::cout << "sell levels after cancel=" << book.sells.size() << '\n';

    return 0;
}
