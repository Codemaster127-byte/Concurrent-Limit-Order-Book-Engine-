#include "main.h"

#include <algorithm>

static void add_order(OrderBook* book, Order order) {
    if (order.side == BUY) {
        auto& level = book->buys[order.price];
        level.orders.push_back(order);
        level.total += order.quantity;
    } else {
        auto& level = book->sells[order.price];
        level.orders.push_back(order);
        level.total += order.quantity;
    }
}

std::vector<Trade> process_order(OrderBook* book, Order incoming) {
    std::vector<Trade> trades;

    if (incoming.side == BUY) {
        while (incoming.quantity && !book->sells.empty() &&
               book->sells.begin()->first <= incoming.price) {
            auto level = book->sells.begin();
            auto sell = level->second.orders.begin();
            Quantity quantity = std::min(incoming.quantity, sell->quantity);

            trades.push_back({incoming.id, sell->id, sell->price, quantity});
            incoming.quantity -= quantity;
            sell->quantity -= quantity;
            level->second.total -= quantity;

            if (!sell->quantity) level->second.orders.erase(sell);
            if (level->second.orders.empty()) book->sells.erase(level);
        }
    } else {
        while (incoming.quantity && !book->buys.empty() &&
               book->buys.begin()->first >= incoming.price) {
            auto level = book->buys.begin();
            auto buy = level->second.orders.begin();
            Quantity quantity = std::min(incoming.quantity, buy->quantity);

            trades.push_back({buy->id, incoming.id, buy->price, quantity});
            incoming.quantity -= quantity;
            buy->quantity -= quantity;
            level->second.total -= quantity;

            if (!buy->quantity) level->second.orders.erase(buy);
            if (level->second.orders.empty()) book->buys.erase(level);
        }
    }

    if (incoming.quantity) add_order(book, incoming);
    return trades;
}
