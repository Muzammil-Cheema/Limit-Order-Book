//
// Created by Muzammil Cheema on 6/3/26.
//

#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <string>
#include <unordered_map>
#include <map>
#include <list>
#include <optional>
#include <expected>
#include "global.h"
#include "order.h"

class OrderBook {
	std::string ticker;

	/*
	 * Maps order IDs to an iterator of the existing Order object stored in either the <asks> or <bids> list.
	 * Only holds resting orders so we can support constant cancellation and querying.
	 */
	std::unordered_map<Id, std::list<Order>::iterator> resting_orders;

	/*
	 * Maps order IDs to Order objects that are completed (FILLED, CANCELLED, REJECTED).
	 * Will be used for referencing Orders, logging, and for Trades to reference orders
	 */
	std::unordered_map<Id, Order> historical_orders;

	// Map buy and sell orders separately. Map price level to a list of orders at that price level in ascending arrival order.
	std::map<Price, std::list<Order>> asks;
	std::map<Price, std::list<Order>> bids;

	/*
	 * Determine which map to traverse, and which price level to observe, modify, or create, based on the order argument.
	 *
	 * @param order: the new Order object that we want to fill or rest.
	 *
	 * @return: optionally return an iterator to the first matching order from the <asks> or <bids> map if any match
	 * is found, or std::nullopt if no matches are found.
	 *
	 * Note: for MARKET orders, a match for an incoming SELL is the highest BUY and for BUY orders is the lowest SELL.
	 * For LIMIT orders, a match is an order on the opposite side and at a price less than or equal to the incoming
	 * for a new BUY order or a price greater than or equal to the incoming for a SELL. If no match is found for
	 * MARKET orders, they are unfilled. For LIMIT orders that are partially filled or unfilled, they rest in the
	 * corresponding map and price level list.
	 */
	std::optional<std::list<Order>::iterator> find_match_market(Order& order);

public:
	explicit OrderBook(std::string ticker);

	/*
	 * Place a new order.
	 * If the combination of input values is invalid, returns std::unexpected.
	 * First, Order object is constructed here and is temporarily owned here before moving to historical_orders or resting_orders.
	 * Second, we try filling the order immediately. If order is completed, add directly to historical_orders. If
	 * order cannot be completed and can rest, add to a price level list. Else, cancel remainder and add order to historical_orders.
	 *
	 * @param price: the price the order is looking for. Only relevant for LIMIT orders. Ignored for MARKET orders.
	 * @param shares: the number of shares the caller wants to buy or sell
	 * @param type: either a MARKET order or a LIMIT order
	 * @param side: either a SELL order or a BUY order
	 *
	 * @return: If the order is invalid or cannot rest or be partially filled, return a std::unexpected<ORDER_BOOK_ERROR_CODE>. Otherwise, return an
	 * ORDER_STATE_T that describes the state of the Order after the function completes. Valid values include "CANCELLED", "PARTIAL", "FILLED".
	 */
	[[nodiscard]] std::expected<ORDER_STATE_T, ORDER_BOOK_ERROR_CODE> placeOrder(ORDER_SIDE_T side, ORDER_TYPE_T type, Share shares,
	std::optional<Price> price);

	/*
	 * Cancel a resting order.
	 * If an order exists in the "resting_orders" map and either the "asks" or "bids" map and has status "OPEN" or
	 * "PARTIAL", it can be cancelled. Else, the order cannot be cancelled.
	 *
	 * @param order_id: the Id of the order that the caller wants to cancel.
	 *
	 * @return: false if an order with the given Id cannot be found, or if the order is not cancellable (see above
	 * requirements). Otherwise, returns true.
	 */
	bool cancelOrder(Id order_id);

	/*
	 * Query an existing resting or historical order for up-to-date information.
	 * The Order object reference returned should not be kept long-term as the underlying object may be changed by
	 * other operations done on the OrderBook after the function call.
	 *
	 * @param order_id: the Id of the order that the caller wants to cancel.
	 *
	 * @return: a const reference to the Order object if found, or throws std::invalid_argument otherwise.
	 */
	[[nodiscard]] std::optional<const Order&> getOrder(Id order_id) const;
};

#endif //ORDER_BOOK_H
