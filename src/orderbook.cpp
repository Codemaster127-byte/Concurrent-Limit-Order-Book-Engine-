#include "main.h"

#include <algorithm>

static void add_order(OrderBook* book, Order order) {
    if (order.side == BUY) {
        auto& level = book->buys[order.price];
        level.orders.push_back(order);
        level.total += order.quantity;
        book->by_id[order.id] = {order.side, order.price, std::prev(level.orders.end())};
    } else {
        auto& level = book->sells[order.price];
        level.orders.push_back(order);
        level.total += order.quantity;
        book->by_id[order.id] = {order.side, order.price, std::prev(level.orders.end())};
    }
}

std::vector<Trade> process_order(OrderBook* book, Order incoming) {
    std::vector<Trade> trades;
    if (!incoming.id || incoming.price <= 0 || !incoming.quantity || book->by_id.contains(incoming.id)) return trades;

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

            if (!sell->quantity) {
                book->by_id.erase(sell->id);
                level->second.orders.erase(sell);
            }
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

            if (!buy->quantity) {
                book->by_id.erase(buy->id);
                level->second.orders.erase(buy);
            }
            if (level->second.orders.empty()) book->buys.erase(level);
        }
    }

    if (incoming.quantity) add_order(book, incoming);
    return trades;
}

bool cancel_order(OrderBook* book, OrderId id) {
    auto found = book->by_id.find(id);
    if (found == book->by_id.end()) return false;
    Location location = found->second;

    if (location.side == BUY) {
        auto level = book->buys.find(location.price);
        level->second.total -= location.order->quantity;
        level->second.orders.erase(location.order);
        if (level->second.orders.empty()) book->buys.erase(level);
    } else {
        auto level = book->sells.find(location.price);
        level->second.total -= location.order->quantity;
        level->second.orders.erase(location.order);
        if (level->second.orders.empty()) book->sells.erase(level);
    }
    book->by_id.erase(found);
    return true;
}

std::vector<Trade> replace_order(OrderBook* book, OrderId id, Price price, Quantity quantity) {
    auto found = book->by_id.find(id);
    if (found == book->by_id.end() || price <= 0 || !quantity) return {};
    Side side = found->second.side;
    cancel_order(book, id);
    return process_order(book, {id, price, quantity, side});
}
