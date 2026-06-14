//
// Created by Muzammil Cheema on 5/29/26.
//

#include "order.h"

// Order::Order() : side(), type(), original_shares(0), remaining_shares(0) {};

Order::Order( const ORDER_SIDE_T side, const ORDER_TYPE_T type, const Share shares, const Price price) :
	 side(side), type(type), original_shares(shares), remaining_shares(shares), price(price) {}

Order::Order(const ORDER_SIDE_T side, const ORDER_TYPE_T type, const Share shares) :
	side(side), type(type), original_shares(shares), remaining_shares(shares) {}

Id Order::get_id() const {
	return id;
}

ORDER_SIDE_T Order::get_side() const {
	return side;
}

ORDER_TYPE_T Order::get_type() const {
	return type;
}

Share Order::get_original_shares() const {
	return original_shares;
}

Share Order::get_remaining_shares() const {
	return remaining_shares;
}

Price Order::get_price() const {
	return price.value();
}

ORDER_STATE_T Order::get_status() const {
	return status;
}

bool Order::decrementShares(const uint64_t shares) {
	if (shares > remaining_shares) [[unlikely]]
		return false;

	remaining_shares -= shares;
	if (remaining_shares == 0)
		status = ORDER_STATE_T::FILLED;
	else
		status = ORDER_STATE_T::PARTIAL;
	return true;
}

bool Order::cancel() {
	if (status == ORDER_STATE_T::OPEN || status == ORDER_STATE_T::CANCELLED || status == ORDER_STATE_T::PARTIAL) {
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