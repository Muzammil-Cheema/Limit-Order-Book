//
// Created by Muzammil Cheema on 6/3/26.
//

#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <string>
#include <unordered_map>
#include <map>
#include <list>
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

public:
	OrderBook(std::string ticker);

	/*
	 * Place a new order.
	 * First, Order object is constructed here and is temporarily owned here before moving to historical_orders or resting_orders.
	 * Second, we try filling the order immediately. If order is completed, add directly to historical_orders. If
	 * order cannot be completed and can rest, add to a price level list. Else, cancel remainder.
	 *
	 * @param price: the price the order is looking for. Only relevant for LIMIT orders. Ignored for MARKET orders.
	 * @param shares: the number of shares the caller wants to buy or sell
	 * @param type: either a MARKET order or a LIMIT order
	 * @param side: either a SELL order or a BUY order
	 *
	 * @return: If the order cannot rest or be filled, return false. Otherwise, return true.
	 */
	bool placeOrder(Price price, Share shares, ORDER_TYPE_T type, ORDER_SIDE_T side);

	void cancelOrder(Id order_id);
};

#endif //ORDER_BOOK_H
