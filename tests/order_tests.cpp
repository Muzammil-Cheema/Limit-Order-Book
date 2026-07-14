//
// Created by Muzammil Cheema on 7/12/26.
//

#include <gtest/gtest.h>
#include "order.h"

TEST(order_unit_tests, test_member_fields_initial_values) {
	Order limitOrder {ORDER_SIDE_T::BUY, ORDER_TYPE_T::LIMIT, 100, 4000};
	Order marketOrder {ORDER_SIDE_T::SELL, ORDER_TYPE_T::MARKET, 100};

	EXPECT_EQ(limitOrder.get_id(), 1000000) << "Unexpected id";
	EXPECT_EQ(marketOrder.get_id(), 1000001) << "Unexpected id";

	EXPECT_LT(limitOrder.get_arrival_time().steady_time, marketOrder.get_arrival_time().steady_time) << "Unexpected steady arrival time";
	EXPECT_LE(limitOrder.get_arrival_time().system_time, marketOrder.get_arrival_time().system_time) << "Unexpected system arrival time";
	EXPECT_EQ(limitOrder.get_completion_time(), std::nullopt) << "Unexpected non-nullopt completion time";
	EXPECT_EQ(marketOrder.get_completion_time(), std::nullopt) << "Unexpected non-nullopt completion time";

	EXPECT_EQ(limitOrder.get_side(), ORDER_SIDE_T::BUY) << "Unexpected side";
	EXPECT_EQ(marketOrder.get_side(), ORDER_SIDE_T::SELL) << "Unexpected side";

	EXPECT_EQ(limitOrder.get_type(), ORDER_TYPE_T::LIMIT) << "Unexpected order type";
	EXPECT_EQ(marketOrder.get_type(), ORDER_TYPE_T::MARKET) << "Unexpected order type";

	EXPECT_EQ(limitOrder.get_original_shares(), 100) << "Unexpected original shares";
	EXPECT_EQ(marketOrder.get_original_shares(), 100) << "Unexpected original shares";

	EXPECT_EQ(limitOrder.get_remaining_shares(), 100) << "Unexpected remaining shares";
	EXPECT_EQ(marketOrder.get_remaining_shares(), 100) << "Unexpected remaining shares";

	EXPECT_EQ(limitOrder.get_price().value(), 4000) << "Unexpected price";
	EXPECT_EQ(marketOrder.get_price(), std::nullopt) << "Unexpected non-nullopt price";

	EXPECT_EQ(limitOrder.get_status(), ORDER_STATE_T::OPEN) << "Unexpected status";
	EXPECT_EQ(marketOrder.get_status(), ORDER_STATE_T::OPEN) << "Unexpected status";
}

TEST(order_unit_tests, test_decrement_shares) {
	Order limitOrder {ORDER_SIDE_T::BUY, ORDER_TYPE_T::LIMIT, 100, 4000};

	EXPECT_EQ(limitOrder.decrementShares(101), false) << "Unexpected decrementShares outcome";
	EXPECT_EQ(limitOrder.get_original_shares(), 100) << "Unexpected original shares";
	EXPECT_EQ(limitOrder.get_remaining_shares(), 100) << "Unexpected remaining shares";
	EXPECT_EQ(limitOrder.get_status(), ORDER_STATE_T::OPEN) << "Unexpected status";

	EXPECT_EQ(limitOrder.decrementShares(12), true) << "Unexpected decrementShares outcome";
	EXPECT_EQ(limitOrder.get_original_shares(), 100) << "Unexpected original shares";
	EXPECT_EQ(limitOrder.get_remaining_shares(), 88) << "Unexpected remaining shares";
	EXPECT_EQ(limitOrder.get_status(), ORDER_STATE_T::PARTIAL) << "Unexpected status";

	EXPECT_EQ(limitOrder.decrementShares(90), false) << "Unexpected decrementShares outcome";
	EXPECT_EQ(limitOrder.get_original_shares(), 100) << "Unexpected original shares";
	EXPECT_EQ(limitOrder.get_remaining_shares(), 88) << "Unexpected remaining shares";
	EXPECT_EQ(limitOrder.get_status(), ORDER_STATE_T::PARTIAL) << "Unexpected status";

	EXPECT_EQ(limitOrder.decrementShares(88), true) << "Unexpected decrementShares outcome";
	EXPECT_EQ(limitOrder.get_original_shares(), 100) << "Unexpected original shares";
	EXPECT_EQ(limitOrder.get_remaining_shares(), 0) << "Unexpected remaining shares";
	EXPECT_EQ(limitOrder.get_status(), ORDER_STATE_T::FILLED) << "Unexpected status";
	EXPECT_NE(limitOrder.get_completion_time(), std::nullopt) << "Unexpected nullopt completion time";
	std::cout << "Completion Time: " << std::endl;
	std::cout << "system (" << limitOrder.get_completion_time()->system_time << ")" << std::endl;
	std::cout << "lifetime (" << limitOrder.get_completion_time()->steady_time - limitOrder.get_arrival_time().steady_time << ")" << std::endl;
	std::cout << std::endl;


	Order marketOrder {ORDER_SIDE_T::SELL, ORDER_TYPE_T::MARKET, 50};
	marketOrder.cancel();

	EXPECT_EQ(marketOrder.decrementShares(20), false) << "Unexpected decrementShares outcome";
}