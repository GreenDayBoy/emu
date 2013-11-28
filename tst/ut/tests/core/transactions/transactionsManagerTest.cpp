#include <core/transactions/manager.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using eMU::core::transactions::Transaction;
using eMU::core::transactions::Manager;

class TransactionsManagerTest: public ::testing::Test
{
protected:
    class SimpleTransaction: public Transaction
    {
    public:
        SimpleTransaction(size_t &count):
            count_(count) {}

    private:
        bool isValid() const
        {
            return true;
        }

        void handleValid()
        {
            ++count_;
        }

        size_t &count_;
    };

    Manager transactionsManager_;
};

TEST_F(TransactionsManagerTest, AllTransactionsShouldBeHandled)
{
    size_t count = 0;
    SimpleTransaction *transaction1 = new SimpleTransaction(count);
    transactionsManager_.queue(transaction1);

    SimpleTransaction *transaction2 = new SimpleTransaction(count);
    transactionsManager_.queue(transaction2);

    transactionsManager_.dequeueAll();

    ASSERT_EQ(2, count);
}
