//
// Created by Muzammil Cheema on 5/29/26.
//

#include "order.h"

Order::Order(const Share shares, const Price price, const ORDER_TYPE_T type, const ORDER_SIDE_T side)
	: original_shares(shares), remaining_shares(shares), price(price), type(type), side(side)
	{}

bool Order::decrementShares(const uint64_t shares) {
	if (shares > this->remaining_shares) {
		return false;
	}
	this->remaining_shares -= shares;;
	return true;
}

bool Order::cancel() {
	if (status == ORDER_STATE_T::OPEN || status == ORDER_STATE_T::CANCELLED) {
		this->status = ORDER_STATE_T::CANCELLED;
		return true;
	}
	return false;
}

// bool Order::reject() {
// 	if (status == ORDER_STATE_T::ACTIVE || status == ORDER_STATE_T::REJECTED) {
// 		this->status = ORDER_STATE_T::REJECTED;
// 		return true;
// 	}
// 	return false;
// }

void Order::updateArrivalTime() {
	arrival_time = std::chrono::steady_clock::now();
	arrival_time_system = std::chrono::system_clock::now();
}

void Order::updateFillTime() {
	fill_time = std::chrono::steady_clock::now();
	fill_time_system = std::chrono::system_clock::now();
}