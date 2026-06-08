//
// Created by Muzammil Cheema on 6/2/26.
//

#include "trade.h"

Trade::Trade(Id buy_order_id, Id sell_order_id, Share shares) :
		id(++trade_counter),
		trade_time(std::chrono::steady_clock::now()),
		trade_time_system(std::chrono::system_clock::now()),
		buy_order_id(buy_order_id),
		sell_order_id(sell_order_id),
		shares(shares)
{}