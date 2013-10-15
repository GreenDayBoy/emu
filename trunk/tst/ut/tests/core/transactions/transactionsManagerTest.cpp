#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/transactions/manager.hpp>
#include <ut/env/core/transactions/transactionMock.hpp>

namespace transactions = eMU::core::transactions;
namespace transactionsEnv = eMU::ut::env::core::transactions;

using ::testing::_;
using ::testing::Return;


class TransactionsManagerTest: public ::testing::Test
{
public:
    transactions::Manager transactionsManager_;

    bool scenario(const std::vector<bool> &transactionsValidationResult)
    {
        for(bool validationResult : transactionsValidationResult)
        {
            transactionsEnv::TransactionMock *transaction = new transactionsEnv::TransactionMock();
            EXPECT_CALL(*transaction, isValid()).WillOnce(Return(validationResult));

            if(validationResult)
            {
                EXPECT_CALL(*transaction, handle());
            }

            transactionsManager_.queue(transaction);
        }

        return transactionsManager_.dequeueAll();
    }
};

TEST_F(TransactionsManagerTest, whenAllTransactionsWereValidThenDequeueAllShouldReturnTrue)
{

    ASSERT_TRUE(scenario({true, true, true, true}));
}

TEST_F(TransactionsManagerTest, whenAnyOfTransactionWasNotValidThenDequeueAllShouldReturnFalse)
{
    ASSERT_FALSE(scenario({true, false, true, true}));
    ASSERT_FALSE(scenario({false, false, false, false}));
}

