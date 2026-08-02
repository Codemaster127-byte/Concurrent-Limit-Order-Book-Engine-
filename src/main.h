#pragma once

#include <cstdint>
#include <list>
#include <map>
#include <vector>

using OrderId = std::uint64_t;
using Price = std::int64_t;
using Quantity = std::uint64_t;

enum Side { BUY, SELL };

struct Order {
    OrderId id;
    Price price;
    Quantity quantity;
    Side side;
};

struct PriceLevel {
    Quantity total{};
    std::list<Order> orders;
};

struct Trade {
    OrderId buy_id;
    OrderId sell_id;
    Price price;
    Quantity quantity;
};

struct OrderBook {
    std::map<Price, PriceLevel, std::greater<Price>> buys;
    std::map<Price, PriceLevel> sells;
};

std::vector<Trade> process_order(OrderBook* book, Order order);
