//
// Created by Muzammil Cheema on 6/2/26.
//

#include <optional>
#include <expected>
#include "order_book.h"

// ===========================================================
// ===================== HELPERS =============================
// ===========================================================

std::optional<std::list<Order>::iterator> OrderBook::find_match_market(Order& order) {
	// Assume MARKET BUY for now
	auto& map_ref = order.get_side() == ORDER_SIDE_T::BUY ? asks : bids;
	if (map_ref.empty())	// If no orders of opposite side exist.
		return std::nullopt

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

