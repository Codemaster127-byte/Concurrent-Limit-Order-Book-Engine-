# Concurrent Limit Order Book Engine

A concurrent **limit order book matching engine** implemented in **C**, designed to simulate real-time financial order processing under multi-threaded workloads.

The system models a simplified trading environment where multiple producer threads generate buy and sell orders that are processed by a dedicated matching engine. The architecture combines **producer–consumer synchronization** with a **lock-free single-writer order book**, enabling efficient order processing with minimal synchronization overhead.

This project demonstrates how operating system concepts such as **multithreading, synchronization, and concurrent workload scaling** influence performance in real-time event-driven systems.

---

# System Architecture

The system follows a **producer–consumer pipeline**.

```
Producer Threads
        │
        ▼
Circular Queue (Mutex + Condition Variables)
        │
        ▼
Matching Engine (Single Writer)
        │
        ▼
Order Book
        │
        ▼
Metrics & Benchmarking
```

### Key Design Principle

Only the matching engine modifies the order book.

Because of this **single-writer architecture**, the order book and matching logic operate **without locks**, eliminating contention and simplifying concurrency control.

---

# Core Components

## Producer Threads

Multiple producer threads simulate market participants submitting orders to the system.

Each producer repeatedly:

1. Generates a random order
2. Pushes the order into the shared queue
3. Waits for a random inter-arrival time (Poisson process simulation)

Each order contains:

- Order ID
- Price
- Quantity
- Side (BUY / SELL)
- Timestamp

---

## Incoming Order Queue

Orders are passed from producers to the matching engine through a **bounded circular buffer**.

The queue is synchronized using:

- `pthread_mutex_t`
- `pthread_cond_t`

This implements the classical **producer–consumer bounded buffer problem**.

### Responsibilities

- Prevent buffer overflow
- Synchronize producer and consumer threads
- Maintain FIFO ordering of incoming orders

---

## Matching Engine

The matching engine is a **single consumer thread** responsible for processing orders.

Responsibilities:

- Dequeue orders from the queue
- Match orders against the order book
- Insert unmatched orders
- Record performance metrics

Since only the matching engine modifies the order book, the matching logic operates **without locks**.

---

## Order Book

The order book stores unmatched orders using two structures:

- **Buy orders** sorted by descending price
- **Sell orders** sorted by ascending price

Matching follows **price-time priority**.

### Matching Conditions

Buy Order:

```
buy_price >= best_sell_price
```

Sell Order:

```
sell_price <= best_buy_price
```

Trades occur until either:

- the incoming order is fully matched
- or no compatible order remains in the book

---

# Order Arrival Model

Producer threads simulate realistic market behavior using a **Poisson arrival process**.

Instead of generating orders at fixed intervals, the time between orders follows an **exponential distribution**, producing bursty arrival patterns similar to real financial markets.

This creates realistic workloads where orders may arrive in clusters or with random gaps.

---

# Performance Metrics

The system records several metrics to evaluate performance.

## Throughput

Number of orders processed per second.

```
throughput = processed_orders / runtime
```

---

## Latency

Latency measures the time between order creation and order processing.

```
latency = processing_time - order.timestamp
```

Timing is measured using:

```
clock_gettime(CLOCK_MONOTONIC)
```

---

## Tail Latency

To better understand performance under load, the system records:

- **p50 latency**
- **p95 latency**
- **p99 latency**

Tail latency is critical in low-latency systems because occasional spikes can significantly affect system behavior.

---

# Experimental Evaluation

System performance is evaluated under varying levels of concurrency.

Producer thread configurations tested:

```
1 producer
2 producers
4 producers
8 producers
16 producers
```

This allows analysis of system scalability as workload increases.

---

# Workload Scenarios

The system supports multiple simulated market conditions.

### Balanced Market

```
50% buy orders
50% sell orders
```

### Buy-Heavy Market

```
80% buy orders
20% sell orders
```

### Sell-Heavy Market

```
20% buy orders
80% sell orders
```

### Burst Traffic

Short spikes of high order arrival rates.

---

# Operating System Concepts Demonstrated

This project applies several core operating system concepts:

- POSIX thread creation and management
- Producer–consumer synchronization
- Mutex locks and condition variables
- Concurrent workload generation
- Lock-free single-writer architectures
- Performance benchmarking of concurrent systems

---

# Project Structure

```
src/

main.c
order.h
order.c

queue.h
queue.c

orderbook.h
orderbook.c

matching_engine.c
producer.c

metrics.c
```

---

# Future Improvements

Possible extensions include:

- Lock-free queue implementation
- Price-level order book structures
- Market microstructure simulations
- Distributed matching engines
- Performance visualization tools

---

# Purpose

This project explores **concurrent system design in low-latency environments** and demonstrates how synchronization strategies influence performance in event-driven architectures.

It was developed as a systems programming exercise focused on **multithreading, synchronization, and performance benchmarking** in C.
