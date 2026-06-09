//
// Created by Muzammil Cheema on 6/2/26.
//

#include <optional>
#include <expected>
#include "order_book.h"

OrderBook::OrderBook(std::string ticker) : ticker(std::move(ticker)) {}

std::expected<ORDER_STATE_T, PLACE_ORDER_ERROR_CODE> placeOrder(const ORDER_SIDE_T side, const ORDER_TYPE_T type,
const Share shares, const std::optional<Price> price) {
	//Validate that price exists for LIMIT orders only and not for MARKET orders
	if ( (!price && type == ORDER_TYPE_T::LIMIT) || (price && type == ORDER_TYPE_T::MARKET) ) {
		return std::unexpected(PLACE_ORDER_ERROR_CODE::INVALID_INPUT);
	}

	if (type == ORDER_TYPE_T::MARKET) {
		Order order(side, type, shares);

		//TODO Use iterator to traverse over correct map and price level
	}
	else if (type == ORDER_TYPE_T::LIMIT) {
		Order order(side, type, shares, price.value());

		//TODO Use iterator to traverse over correct map and price level
	}

	return ORDER_STATE_T::FILLED;
}

bool OrderBook::cancelOrder(const Id order_id) {
	return false;
}

auto OrderBook::getOrder(const Id order_id) const {
	if (const auto iter = resting_orders.find(order_id); iter != resting_orders.end()) {
		return *(iter->second);
	}

	if (const auto iter = historical_orders.find(order_id); iter != historical_orders.end()) {
		return iter->second;
	}

	return std::nullopt;
}

