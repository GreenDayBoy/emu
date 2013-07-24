#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/transactions/transaction.hpp>

using ::testing::_;

class TransactionTest: public ::testing::Test {
public:
    class SimpleTransaction: public eMU::core::transactions::Transaction {
    public:
        SimpleTransaction(bool isValid):
          isValid_(isValid),
          executed_(false) {}

        bool validate() const {
            return isValid_;
        }

        void handleSelf() {
            executed_ = true;
        }

        bool executed() const {
            return executed_;
        }

    private:
        bool isValid_;
        bool executed_;
    };
};

TEST_F(TransactionTest, TransactionShouldBeExecutedWhenValidateIsSucceed) {
    SimpleTransaction transaction(true);
    transaction.handle();

    EXPECT_TRUE(transaction.executed());
}

TEST_F(TransactionTest, TransactionShouldNotBeExecutedWhenValidationIsFailed) {
    SimpleTransaction transaction(false);
    transaction.handle();

    EXPECT_FALSE(transaction.executed());
}
