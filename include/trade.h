//
// Created by Muzammil Cheema on 6/2/26.
//

#ifndef TRADE_H
#define TRADE_H

#include <chrono>
#include "global.h"

static uint64_t trade_counter = 1000;

class Trade {
	Id id;

	std::chrono::steady_clock::time_point trade_time;
	std::chrono::system_clock::time_point trade_time_system;

	Id buy_order_id;
	Id sell_order_id;

	Share shares;
	Price price;

public:
	Trade(Id buy_order_id, Id sell_order_id, Share shares, Price price);

	[[nodiscard]] Id getId() const;
	[[nodiscard]] Share getShare() const;
	[[nodiscard]] Price getPrice() const;
};

#endif //TRADE_H
