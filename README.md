# C++20 Order Book Migration (WIP)

This branch is intentionally incomplete. It contains the first part of the C-to-C++ migration: the core order types, RAII containers, FIFO price levels, and basic price-time-priority matching with partial fills.

Implemented here:

- `Order`, `PriceLevel`, `Trade`, and `OrderBook` types
- sorted buy/sell price levels
- FIFO orders at each price
- basic partial-fill matching
- a tiny executable demo

Still being developed on feature branches:

- order-ID cancellation and cancel/replace
- bounded multi-producer queue and single-writer engine
- reference implementation and differential tests
- sanitizers, benchmarks, and regression baselines

```sh
make
./build/orderbook
```
