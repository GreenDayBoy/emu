#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/transactions/manager.hpp>
#include <ut/env/core/transactions/transactionMock.hpp>

namespace transactions = eMU::core::transactions;
namespace transactionsEnv = eMU::ut::env::core::transactions;

using ::testing::_;


class TransactionsManagerTest: public ::testing::Test {
public:
    transactions::Manager transactionsManager_;
};

TEST_F(TransactionsManagerTest, allQueuedTransactionsShouldBeExecuted) {
    transactionsEnv::TransactionMock *transaction1 = new transactionsEnv::TransactionMock();
    EXPECT_CALL(*transaction1, handleSelf());
    transactionsManager_.queue(transaction1);

    transactionsEnv::TransactionMock *transaction2 = new transactionsEnv::TransactionMock();
    EXPECT_CALL(*transaction2, handleSelf());
    transactionsManager_.queue(transaction2);

    transactionsEnv::TransactionMock *transaction3 = new transactionsEnv::TransactionMock();
    EXPECT_CALL(*transaction3, handleSelf());
    transactionsManager_.queue(transaction3);

    transactionsManager_.dequeueAll();
}
