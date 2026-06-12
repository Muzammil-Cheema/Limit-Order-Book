//
// Created by Muzammil Cheema on 5/26/26.
//

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>
#include "global.h"

static uint64_t order_counter = 1000000;

class Order {
	Id id = ++order_counter;

	std::chrono::steady_clock::time_point arrival_time;
	std::chrono::steady_clock::time_point fill_time;
	std::chrono::system_clock::time_point arrival_time_system;
	std::chrono::system_clock::time_point fill_time_system;

	ORDER_SIDE_T side;
	ORDER_TYPE_T type;

	Share original_shares;
	Share remaining_shares;
	std::optional<Price> price;

	ORDER_STATE_T status = ORDER_STATE_T::INITIAL;

public:
	Order(ORDER_SIDE_T side, ORDER_TYPE_T type, Share shares, Price price);
	Order(ORDER_SIDE_T side, ORDER_TYPE_T type, Share shares);

	[[nodiscard]] ORDER_SIDE_T get_side() const;
	[[nodiscard]] ORDER_TYPE_T get_type() const;

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

	// /*
	//  * Reject orders that cannot be filled or rested (all MARKET orders when there are no corresponding resting orders.
	// */
	// bool reject();

	/*
	 * Run when Order is added to OrderBook.
	 */
	void updateArrivalTime();

	/*
	 * Run when Order is filled and removed from OrderBook
	 */
	void updateFillTime();

};

#endif //ORDER_H