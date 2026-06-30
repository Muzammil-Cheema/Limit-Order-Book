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

		//Remove resting order from either bids or asks
		if (it->get_side() == ORDER_SIDE_T::BUY) {
			bids[trade_price].pop_front();
			if (bids[trade_price].empty())
				bids.erase(trade_price);
		}
		else if (it->get_side() == ORDER_SIDE_T::SELL) {
			asks[trade_price].pop_front();
			if (asks[trade_price].empty())
				asks.erase(trade_price);
		}
	}

	return order.get_status() == ORDER_STATE_T::FILLED;
}


std::expected<bool, ORDER_BOOK_ERROR_CODE> OrderBook::find_match_and_fill_order(Order& order) {
	bool filled = false;

	while (!filled) {
		std::expected<std::list<Order>::iterator, ORDER_BOOK_ERROR_CODE> res_find_match;
		if (order.get_side() == ORDER_SIDE_T::BUY)
			res_find_match = find_match(order, asks);
		else
			res_find_match = find_match(order, bids);

		if (!res_find_match) {
			if (res_find_match.error() == ORDER_BOOK_ERROR_CODE::NO_MATCHING_ORDER)
				return false;
			return std::unexpected(res_find_match.error());
		}
		auto match = res_find_match.value();

		auto res_fill_order = fill_order(order, match);
		if (!res_fill_order)
			return std::unexpected(res_fill_order.error());
		filled = res_fill_order.value();
	}

	return filled;
}

// ===========================================================
// ======================= API ===============================
// ===========================================================


OrderBook::OrderBook(std::string ticker) : ticker(std::move(ticker)) {}


std::expected<ORDER_STATE_T, ORDER_BOOK_ERROR_CODE> OrderBook::placeOrder(const ORDER_SIDE_T side, const ORDER_TYPE_T type,
const Share shares, const std::optional<Price> price)
{
	//Validate that price exists for LIMIT orders only and not for MARKET orders
	if ( (!price && type == ORDER_TYPE_T::LIMIT) || (price && type == ORDER_TYPE_T::MARKET) ) {
		return std::unexpected(ORDER_BOOK_ERROR_CODE::INVALID_INPUT);
	}
	if (shares == 0 || (price.has_value() && price.value() == 0))
		return std::unexpected(ORDER_BOOK_ERROR_CODE::INVALID_INPUT);

	Order order = type == ORDER_TYPE_T::LIMIT ? Order(side, type, shares, price.value()) : Order(side, type, shares);

	auto res_match_and_fill = find_match_and_fill_order(order);
	if (!res_match_and_fill)
		return std::unexpected(res_match_and_fill.error());
	if (res_match_and_fill.value()) {
		order.updateCompleteTime();
		return ORDER_STATE_T::FILLED;
	}

	if (type == ORDER_TYPE_T::MARKET) {
		order.cancel();
		historical_orders.emplace(order.get_id(), order);

		if (order.get_original_shares() != order.get_remaining_shares())
			return ORDER_STATE_T::PARTIAL;
		return ORDER_STATE_T::CANCELLED;
	}
	if (type == ORDER_TYPE_T::LIMIT) {
		if (order.get_side() == ORDER_SIDE_T::BUY) {
			bids[order.get_price()].push_back(order);
			resting_orders[order.get_id()] = std::prev(bids[order.get_price()].end());
		}
		else {
			asks[order.get_price()].push_back(order);
			resting_orders[order.get_id()] = std::prev(asks[order.get_price()].end());
		}

		if (order.get_original_shares() != order.get_remaining_shares())
			return ORDER_STATE_T::PARTIAL;
		return ORDER_STATE_T::OPEN;
	}

	return std::unexpected(ORDER_BOOK_ERROR_CODE::UNKNOWN_ERROR);
}


bool OrderBook::cancelOrder(const Id order_id) {
	if (!resting_orders.contains(order_id))
		return false;

	Order &order = *(resting_orders[order_id]);
	Price price = order.get_price();
	order.cancel();
	historical_orders.emplace(order.get_id(), order);

	if (order.get_side() == ORDER_SIDE_T::BUY) {
		bids[price].erase(resting_orders[order_id]);
		if (bids[price].empty())
			bids.erase(price);
	}
	else {
		asks[price].erase(resting_orders[order_id]);
		if (asks[price].empty())
			asks.erase(price);
	}

	resting_orders.erase(order_id);
	return true;
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