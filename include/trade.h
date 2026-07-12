//
// Created by Muzammil Cheema on 6/2/26.
//

#ifndef TRADE_H
#define TRADE_H

#include <chrono>
#include "global.h"

class Trade {
	Id id = generateId();

	TimeStamp trade_time {std::chrono::steady_clock::now(), std::chrono::system_clock::now()};

	Id buy_order_id;
	Id sell_order_id;

	Share shares;
	Price price;

	[[nodiscard]] static Id generateId() {
		static Id id = 1000000;
		return ++id;
	}

public:
	Trade(Id buy_order_id, Id sell_order_id, Share shares, Price price);

	[[nodiscard]] Id getId() const;
	[[nodiscard]] Share getShare() const;
	[[nodiscard]] Price getPrice() const;
	[[nodiscard]] Id getBuyOrderId() const;
	[[nodiscard]] Id getSellOrderId() const;
	[[nodiscard]] TimeStamp get_trade_time() const;

};

#endif //TRADE_H
