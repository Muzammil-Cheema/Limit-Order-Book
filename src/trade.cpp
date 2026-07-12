//
// Created by Muzammil Cheema on 6/2/26.
//

#include "trade.h"

Trade::Trade(const Id buy_order_id, const Id sell_order_id, const Share shares, const Price price) :
	buy_order_id(buy_order_id),
	sell_order_id(sell_order_id),
	shares(shares),
	price(price)
{}

Id Trade::getId() const {
	return id;
}

Share Trade::getShare() const {
	return shares;
}

Price Trade::getPrice() const {
	return price;
}

Id Trade::getBuyOrderId() const {
	return buy_order_id;
}

Id Trade::getSellOrderId() const {
	return sell_order_id;
}

TimeStamp Trade::get_trade_time() const {
	return trade_time;
}