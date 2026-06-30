//
// Created by Muzammil Cheema on 5/26/26.
//

#ifndef ORDER_H
#define ORDER_H

#include <chrono>
#include "global.h"

class Order {
	Id id = generateId();

	std::chrono::steady_clock::time_point arrival_time;
	std::chrono::system_clock::time_point arrival_time_system;
	std::optional<std::chrono::steady_clock::time_point> complete_time = std::nullopt;
	std::optional<std::chrono::system_clock::time_point> complete_time_system = std::nullopt;

	ORDER_SIDE_T side;
	ORDER_TYPE_T type;

	Share original_shares;
	Share remaining_shares;
	std::optional<Price> price;

	ORDER_STATE_T status = ORDER_STATE_T::OPEN;

	[[nodiscard]] static Id generateId() {
		static Id id = 1000000;
		//TODO mutex lock
		++id;
		//TODO mutex unlock
		return id;
	}

public:
	Order(ORDER_SIDE_T side, ORDER_TYPE_T type, Share shares, Price price);
	Order(ORDER_SIDE_T side, ORDER_TYPE_T type, Share shares);

	Order& operator=(const Order&) = delete;

	[[nodiscard]] Id get_id() const;
	[[nodiscard]] ORDER_SIDE_T get_side() const;
	[[nodiscard]] ORDER_TYPE_T get_type() const;
	[[nodiscard]] Share get_original_shares() const;
	[[nodiscard]] Share get_remaining_shares() const;
	[[nodiscard]] Price get_price() const;
	[[nodiscard]] ORDER_STATE_T get_status() const;
	template <typename TTimePoint>
	[[nodiscard]] std::array<TTimePoint, 2> get_arrival_time() const;
	template <typename TTimePoint>
	[[nodiscard]] std::optional<std::array<TTimePoint, 2>> get_complete_time() const;

	/*
	 * Decrement remaining shares from order and updates status.
	 *
	 * @param shares: the number of shares we are trying to fill with this order. Valid values are [0, .share
	 * value before function call].
	 *
	 * @return: returns true if the operation was successful and false otherwise.
	 */
	bool decrementShares(Share shares);

	/*
	 * Cancel the trade if none of it has been filled yet. Filled or partially filled orders cannot be cancelled.
	 *
	 * @return: true if the trade was cancelled or false otherwise.
	 */
	bool cancel();

	/*
	 * Run when Order is added to OrderBook.
	 */
	void updateArrivalTime();

	/*
	 * Run when Order is moved into historical orders or when the program terminates gracefully. The order can be
	 * CANCELLED or FILLED at this point.
	 */
	void updateCompleteTime();

};

#endif //ORDER_H