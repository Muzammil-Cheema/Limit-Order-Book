# Limit Order Book and Matching Engine

A C++ limit order book built to study how an exchange matching engine represents orders, preserves market priority, 
and turns incoming order flow into trades. The project currently emphasizes a small, inspectable design with 
predictable operations rather than a feature-heavy trading application.

## Design Focus

- **Price-time priority:** asks are ordered from lowest to highest price, bids from highest to lowest, and orders at a shared price are processed FIFO.
- **Fast order handling:** an order-ID index stores iterators to resting orders, allowing direct lookup and list removal for cancellation. Price-level maintenance remains logarithmic in the number of active price levels.
- **Clear ownership and lifetime boundaries:** incoming orders begin as short-lived local objects, while resting and completed orders are held by the book in distinct containers.
- **Detailed lifecycle timing:** orders record arrival and completion timestamps using both monotonic and wall-clock 
  time; executed trades receive their own timestamps.
- **Simple, data-oriented structures:** ordered price maps (std::map), FIFO lists (std::list), and hash-based lookup 
  tables (std::unordered_map) are used to make the matching path easy to reason about.

## Implemented

- `Order`, `Trade`, and single-ticker `OrderBook` classes.
- Market and limit order placement, price-time-priority matching, partial fills, trade generation, and cancellation of resting orders.
- Validation of invalid share quantities and prices, order states, historical-order storage, and empty price-level cleanup.
- CMake build configuration with GoogleTest fetched at build time.
- Initial GoogleTest unit coverage for `Order` and `Trade` behavior, including state transitions, identifiers, optional prices, and timestamps.

## In Progress and Planned

- Order-book scenario tests and end-to-end order-flow integration tests.
- A command-line interface for driving and inspecting the engine.
- Thread-safe ID generation and a multi-ticker engine layer.
- Concurrent client intake, synchronization strategy evaluation, profiling, and systems-level performance work.
- Historical CSV order-flow replay, TCP networking with a compact binary application layer protocol, and generated live 
  order-flow simulation.

## Learning Objectives

This project is a practical study of modern C++ and market microstructure. It explores containers and iterators, 
object lifetimes and ownership, composition over inheritance, error handling, testing with GoogleTest, and builds 
with CMake. As the engine grows, it will also serve as a vehicle for learning multithreading, processes, stack 
versus heap behavior, profiling, and TCP networking.
