//
// Created by Muzammil Cheema on 6/2/26.
//

#include "order_book.h"
#include <expected>
#include <optional>

// ===========================================================
// ===================== HELPERS =============================
// ===========================================================

template <typename TPriceLevelMap>
std::expected<std::list<Order>::iterator, ORDER_BOOK_ERROR_CODE> OrderBook::find_match(const Order& order, TPriceLevelMap& map) {
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


std::expected<bool, ORDER_BOOK_ERROR_CODE> OrderBook::fill_order(Order &order, const std::list<Order>::iterator it) {
	const Share trade_shares = order.get_remaining_shares() > it->get_remaining_shares() ? it->get_remaining_shares() : order.get_remaining_shares();
	const Price trade_price = it->get_price();

	order.decrementShares(trade_shares);
	it->decrementShares(trade_shares);

	// TODO add new Trade into OrderBook's Trades container
	const Trade trade {
		order.get_side() == ORDER_SIDE_T::BUY ? order.get_id(): it->get_id(),
		order.get_side() == ORDER_SIDE_T::SELL ? order.get_id(): it->get_id(),
		trade_shares,
		trade_price
	};

	//update OrderBook containers (trades, historical_orders, resting_orders, asks, bids)
	auto [trade_iter, inserted] = trades.emplace(trade.getId(), trade);
	if (!inserted)
		return std::unexpected(ORDER_BOOK_ERROR_CODE::UNKNOWN_ERROR);
	if (order.get_status() == ORDER_STATE_T::FILLED) {
		if (auto [it, inserted] = historical_orders.emplace(order.get_id(), order); !inserted)
			return std::unexpected(ORDER_BOOK_ERROR_CODE::UNKNOWN_ERROR);
	}
	if (it->get_status() == ORDER_STATE_T::FILLED) {
		// Copy Order into historical_orders, then remove iterators in resting_orders and finally from either asks or bids.
		if (auto [it_emplace, inserted] = historical_orders.emplace(it->get_id(), *it); !inserted)
			return std::unexpected(ORDER_BOOK_ERROR_CODE::UNKNOWN_ERROR);

		resting_orders.erase(it->get_id());
	}

	return order.get_status() == ORDER_STATE_T::FILLED;
}


// ===========================================================
// ======================= API ===============================
// ===========================================================


OrderBook::OrderBook(std::string ticker) : ticker(std::move(ticker)) {}


std::expected<ORDER_STATE_T, ORDER_BOOK_ERROR_CODE> OrderBook::placeOrder(const ORDER_SIDE_T side, const ORDER_TYPE_T type,
const Share shares, const std::optional<Price> price) {
	//Validate that price exists for LIMIT orders only and not for MARKET orders
	if ( (!price && type == ORDER_TYPE_T::LIMIT) || (price && type == ORDER_TYPE_T::MARKET) ) {
		return std::unexpected(ORDER_BOOK_ERROR_CODE::INVALID_INPUT);
	}

	// TODO use a while loop over fill_order() until it returns true after initially using find_match to get the first resting order to fill.
	if (type == ORDER_TYPE_T::MARKET) {

		Order order(side, type, shares);
		auto res_find_match = find_match(order, order.get_side() == ORDER_SIDE_T::BUY ? asks : bids);
		if (!res_find_match) return std::unexpected(res_find_match.error());
		auto existing_orders_iter = res_find_match.value();
		auto res_fill_order = fill_order(order, existing_orders_iter);
		if (!res_fill_order) return std::unexpected(res_fill_order.error());

		/*TODO instead of using ++existing_orders_iter to find the next match, create an overloaded find_match() that accepts an iterator and
		 * returns the correct next order match based on the order type (MARKET, LIMIT)
		 * Remember that this branch is for MARKET orders
		 * while (!filled) {
		 * next_iterator = find_match(current_iterator);
		 * if (next_iterator != current_iterator){
		 * // cleanup current_iterator
		 * }
		 * if (next_iterator == either_map_or_list.end())
		 * // No orders remaining to match
		 * // cancel remainder
		 * break;
		 * }
		 * current_iterator = next_iterator;
		 * filled = fill_order(order, current_iterator);
		 */
		while (!res_fill_order.value()) {
			auto next_iter = ++existing_orders_iter;
			--existing_orders_iter;
			if (existing_orders_iter->get_side() == ORDER_SIDE_T::BUY)
				bids.erase(existing_orders_iter->get_id());
			else
				asks.erase(existing_orders_iter->get_id());

			res_fill_order = fill_order(order, next_iter);
		}


	}
	else if (type == ORDER_TYPE_T::LIMIT) {
		Order order(side, type, shares, price.value());
		auto res = find_match(order, order.get_side() == ORDER_SIDE_T::BUY ? asks : bids);

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