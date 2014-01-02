#include <core/common/transaction.hpp>

#include <gtest/gtest.h>

using eMU::core::common::Transaction;

class TransactionsTest: public ::testing::Test
{
protected:
    class SimpleTransaction: public Transaction
    {
    public:
        SimpleTransaction(bool validationResult):
            handledValid_(false),
            handledInvalid_(false),
            validationResult_(validationResult) {}

        bool isValidHandled()
        {
            return handledValid_;
        }

        bool isInvalidHandled()
        {
            return handledInvalid_;
        }

    private:
        bool isValid() const
        {
            return validationResult_;
        }

        void handleValid()
        {
            handledValid_ = true;
        }

        void handleInvalid()
        {
            handledInvalid_ = true;
        }

        bool handledValid_;
        bool handledInvalid_;
        bool validationResult_;
    };
};

TEST_F(TransactionsTest, WhenValidationResultIsTrueThenValidScenarioShouldBeHandled)
{
    bool validationResult = true;

    SimpleTransaction transaction(validationResult);
    transaction.handle();

    ASSERT_TRUE(transaction.isValidHandled());
    ASSERT_FALSE(transaction.isInvalidHandled());
}

TEST_F(TransactionsTest, WhenValidationResultIsFalseThenInvalidScenarioShouldBeHandled)
{
    bool validationResult = false;

    SimpleTransaction transaction(validationResult);
    transaction.handle();

    ASSERT_TRUE(transaction.isInvalidHandled());
    ASSERT_FALSE(transaction.isValidHandled());
}
