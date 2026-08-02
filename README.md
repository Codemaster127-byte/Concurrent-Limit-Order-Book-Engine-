# C++20 Order Book Migration (WIP)

This branch is intentionally incomplete. It is roughly 60% through the C-to-C++ migration.

Implemented here:

- `Order`, `PriceLevel`, `Trade`, and `OrderBook` types
- sorted buy/sell price levels
- FIFO orders at each price
- basic partial-fill matching
- order-ID indexed cancellation
- cancel/replace that loses time priority
- a tiny executable demo

Still being developed on feature branches:

- bounded multi-producer queue and single-writer engine
- reference implementation and differential tests
- sanitizers, benchmarks, and regression baselines

```sh
make
./build/orderbook
```
