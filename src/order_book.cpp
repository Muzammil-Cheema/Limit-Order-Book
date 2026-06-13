//
// Created by Muzammil Cheema on 6/2/26.
//

#include <optional>
#include <expected>
#include "order_book.h"

// ===========================================================
// ===================== HELPERS =============================
// ===========================================================


template <typename TPriceLevelMap>
std::expected<std::list<Order>::iterator, ORDER_BOOK_ERROR_CODE> OrderBook::find_match(const Order& order,
TPriceLevelMap& map)
{
	if (map.empty())
		return std::unexpected(ORDER_BOOK_ERROR_CODE::NO_MATCHING_ORDER);

	if (order.get_type() == ORDER_TYPE_T::MARKET)
		return map.begin()->second.begin();

	if (order.get_type() == ORDER_TYPE_T::LIMIT) {
		if ( (order.get_side() == ORDER_SIDE_T::BUY && map.begin()->first <= order.get_price()) ||
			(order.get_side() == ORDER_SIDE_T::SELL && map.begin()->first >= order.get_price()) )
			return map.begin()->second.begin();

		return std::unexpected(ORDER_BOOK_ERROR_CODE::NO_MATCHING_ORDER);
	}

	return std::unexpected(ORDER_BOOK_ERROR_CODE::UNKNOWN_ERROR);
}

// ===========================================================
// ======================= API ===============================
// ===========================================================

OrderBook::OrderBook(std::string ticker) : ticker(std::move(ticker)) {}

std::expected<ORDER_STATE_T, ORDER_BOOK_ERROR_CODE> OrderBook::placeOrder(const ORDER_SIDE_T side, const ORDER_TYPE_T
type, const Share shares, const std::optional<Price> price) {
	//Validate that price exists for LIMIT orders only and not for MARKET orders
	if ( (!price && type == ORDER_TYPE_T::LIMIT) || (price && type == ORDER_TYPE_T::MARKET) ) {
		return std::unexpected(ORDER_BOOK_ERROR_CODE::INVALID_INPUT);
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

std::optional<const Order&> OrderBook::getOrder(const Id order_id) const {
	if (const auto iter = resting_orders.find(order_id); iter != resting_orders.end()) {
		return *(iter->second);
	}

	if (const auto iter = historical_orders.find(order_id); iter != historical_orders.end()) {
		return iter->second;
	}

	return std::nullopt;
}

