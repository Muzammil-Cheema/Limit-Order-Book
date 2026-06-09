//
// Created by Muzammil Cheema on 5/26/26.
//

#ifndef GLOBAL_H
#define GLOBAL_H

using Id = uint64_t;
using Share = uint64_t;
using Price = uint64_t;

enum class ORDER_TYPE_T {
	MARKET,
	LIMIT
};

enum class ORDER_SIDE_T {
	BUY,
	SELL
};

enum class ORDER_STATE_T {
	INITIAL,
	OPEN,
	PARTIAL,
	FILLED,
	CANCELLED,
};



#endif //GLOBAL_H
