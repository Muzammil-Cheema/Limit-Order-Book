//
// Created by Muzammil Cheema on 6/2/26.
//

#include "order_book.h"

#include <utility>

OrderBook::OrderBook(std::string ticker) : ticker(std::move(ticker)) {}

bool OrderBook::placeOrder(Price price, Share shares, ORDER_TYPE_T type, ORDER_SIDE_T side) {
	return false;
}

bool OrderBook::cancelOrder(Id order_id) {
	return false;
}

const Order &OrderBook::getOrder(const Id order_id) const {
	if (const auto iter = resting_orders.find(order_id); iter != resting_orders.end()) {
		return *(iter->second);
	}

	if (const auto iter = historical_orders.find(order_id); iter != historical_orders.end()) {
		return iter->second;
	}

	throw std::invalid_argument("Invalid order id. Order not found.");
}

