//
// Created by Muzammil Cheema on 6/2/26.
//

#include "trade.h"

Trade::Trade(const Id buy_order_id, const Id sell_order_id, const Share shares, const Price price) :
		trade_time(std::chrono::steady_clock::now()),
		trade_time_system(std::chrono::system_clock::now()),
		buy_order_id(buy_order_id),
		sell_order_id(sell_order_id),
		shares(shares),
		price(price)
{};

Id Trade::getId() const {
	return id;
}

Share Trade::getShare() const {
	return shares;
}

Price Trade::getPrice() const {
	return price;
}

template<typename TTimePoint>
std::array<TTimePoint, 2> Trade::get_trade_time() const {
	return std::array<TTimePoint, 2>{trade_time, trade_time_system};
}


